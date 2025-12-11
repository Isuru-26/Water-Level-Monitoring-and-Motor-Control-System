#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "SLT-4G-CC05";
const char* password = "3DA6BLN7LYB";

// Web server on port 80
WebServer server(80);

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Ultrasonic pins (ESP32)
#define TRIGPIN 5
#define ECHOPIN 18

// Buzzer pin
#define BUZZER 23 

// Motor (relay) signal pin
#define MOTOR_PIN 12

// Relay type: set to 1 if your relay is ACTIVE HIGH, set to 0 if ACTIVE LOW
#define RELAY_ACTIVE_HIGH 1  

// Tank and sensor dimensions
#define TANK_HEIGHT    10   // usable water depth (cm)
#define SENSOR_HEIGHT  13   // sensor mounted 13 cm above bottom

// LED pins
#define LED_MIN 25
#define LED_MID 26
#define LED_MAX 27

long duration;
float distance;
int waterLevel;
int percentage;
bool motorState = false;   // remember motor state

// HTML page
const char* htmlPage = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Water Level Monitor</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            margin: 0;
            padding: 20px;
            min-height: 100vh;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: white;
            border-radius: 15px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.2);
            padding: 30px;
        }
        .header {
            text-align: center;
            color: #333;
            margin-bottom: 30px;
        }
        .stats-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        .stat-card {
            background: #f8f9fa;
            border-radius: 10px;
            padding: 25px;
            text-align: center;
            border-left: 5px solid #007bff;
            transition: transform 0.2s;
        }
        .stat-card:hover {
            transform: translateY(-5px);
        }
        .stat-value {
            font-size: 2.5em;
            font-weight: bold;
            color: #007bff;
            margin: 10px 0;
        }
        .stat-label {
            color: #666;
            font-size: 1.1em;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        .tank-visual {
            text-align: center;
            margin: 30px 0;
        }
        .tank {
            width: 200px;
            height: 300px;
            border: 4px solid #333;
            border-radius: 0 0 20px 20px;
            margin: 0 auto;
            position: relative;
            background: #f0f0f0;
            overflow: hidden;
        }
        .water {
            position: absolute;
            bottom: 0;
            left: 0;
            width: 100%;
            background: linear-gradient(45deg, #4facfe 0%, #00f2fe 100%);
            transition: height 0.5s ease;
            border-radius: 0 0 16px 16px;
        }
        .water::before {
            content: '';
            position: absolute;
            top: -10px;
            left: 0;
            width: 100%;
            height: 20px;
            background: linear-gradient(45deg, #4facfe 0%, #00f2fe 100%);
            border-radius: 50%;
            animation: wave 2s ease-in-out infinite;
        }
        @keyframes wave {
            0%, 100% { transform: translateX(0) scale(1); }
            50% { transform: translateX(10px) scale(1.1); }
        }
        .status {
            margin: 20px 0;
            padding: 15px;
            border-radius: 8px;
            text-align: center;
            font-weight: bold;
            font-size: 1.2em;
        }
        .status.low {
            background: #f8d7da;
            color: #721c24;
            border: 1px solid #f5c6cb;
        }
        .status.medium {
            background: #fff3cd;
            color: #856404;
            border: 1px solid #ffeaa7;
        }
        .status.high {
            background: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
        }
        .last-update {
            text-align: center;
            color: #666;
            font-size: 0.9em;
            margin-top: 20px;
        }
        .loading {
            text-align: center;
            color: #666;
            padding: 20px;
        }
    </style>
</head>
<body>
    <div class='container'>
        <div class='header'>
            <h1>💧 Water Level Monitor</h1>
            <p>Real-time water tank monitoring system</p>
        </div>
        
        <div class='stats-grid'>
            <div class='stat-card'>
                <div class='stat-label'>Water Level</div>
                <div class='stat-value' id='percentage'>--</div>
                <div class='stat-label'>Percentage</div>
            </div>
            <div class='stat-card'>
                <div class='stat-label'>Distance</div>
                <div class='stat-value' id='distance'>--</div>
                <div class='stat-label'>Centimeters</div>
            </div>
            <div class='stat-card'>
                <div class='stat-label'>Water Height</div>
                <div class='stat-value' id='waterHeight'>--</div>
                <div class='stat-label'>Centimeters</div>
            </div>
        </div>

        <div class='tank-visual'>
            <div class='tank'>
                <div class='water' id='waterLevel'></div>
            </div>
            <div class='status' id='status'>Loading...</div>
        </div>

        <div class='last-update' id='lastUpdate'>
            Last updated: Never
        </div>
    </div>

    <script>
        function updateData() {
            fetch('/data')
                .then(response => response.json())
                .then(data => {
                    // Update values
                    document.getElementById('percentage').textContent = data.percentage + '%';
                    document.getElementById('distance').textContent = data.distance.toFixed(1) + ' cm';
                    document.getElementById('waterHeight').textContent = data.waterLevel + ' cm';
                    
                    // Update visual tank
                    const waterElement = document.getElementById('waterLevel');
                    waterElement.style.height = data.percentage + '%';
                    
                    // Update status
                    const statusElement = document.getElementById('status');
                    statusElement.className = 'status';
                    
                    if (data.percentage <= 30) {
                        statusElement.classList.add('low');
                        statusElement.textContent = '⚠️ LOW WATER LEVEL';
                    } else if (data.percentage <= 70) {
                        statusElement.classList.add('medium');
                        statusElement.textContent = '📊 MEDIUM WATER LEVEL';
                    } else {
                        statusElement.classList.add('high');
                        statusElement.textContent = '✅ TANK FULL';
                    }
                    
                    // Update timestamp
                    document.getElementById('lastUpdate').textContent = 
                        'Last updated: ' + new Date().toLocaleTimeString();
                })
                .catch(error => {
                    console.error('Error fetching data:', error);
                    document.getElementById('status').textContent = '❌ CONNECTION ERROR';
                });
        }

        // Update every 2 seconds
        setInterval(updateData, 2000);
        
        // Initial load
        updateData();
    </script>
</body>
</html>
)";

// Function to get averaged distance
float getDistance() {
  float sum = 0;
  int validSamples = 0;
  int totalSamples = 10;

  for (int i = 0; i < totalSamples; i++) {
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);

    duration = pulseIn(ECHOPIN, HIGH, 20000UL); 
    float d = duration * 0.0343 / 2;

    if (d > 0 && d < 100) {
      sum += d;
      validSamples++;
    }
    delay(10);
  }

  return (validSamples > 0) ? (sum / validSamples) : 0;
}

// Web server handlers
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleData() {
  // Create JSON response
  StaticJsonDocument<200> json;
  json["distance"] = distance;
  json["waterLevel"] = waterLevel;
  json["percentage"] = percentage;
  json["timestamp"] = millis();
  
  String response;
  serializeJson(json, response);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", response);
}

void setup() {
  Serial.begin(115200);

  // Pin setup
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  pinMode(LED_MIN, OUTPUT);
  pinMode(LED_MID, OUTPUT);
  pinMode(LED_MAX, OUTPUT);

  pinMode(MOTOR_PIN, OUTPUT);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Display IP address on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected!");
  display.setCursor(0, 20);
  display.println("IP Address:");
  display.setCursor(0, 35);
  display.println(WiFi.localIP());
  display.display();
  delay(3000);

  // Web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
  
  Serial.println("Web server started!");
  Serial.println("Open your browser and go to: http://" + WiFi.localIP().toString());
}

void loop() {
  // Handle web server requests
  server.handleClient();

  // Get sensor data
  distance = getDistance();
  waterLevel = SENSOR_HEIGHT - distance;
  waterLevel = constrain(waterLevel, 0, TANK_HEIGHT);
  percentage = map(waterLevel, 0, TANK_HEIGHT, 0, 100);

  // Debug output
  Serial.print("Distance: ");
  Serial.print(distance, 1);
  Serial.print(" cm | Water Level: ");
  Serial.print(waterLevel);
  Serial.print(" cm (");
  Serial.print(percentage);
  Serial.println("%)");

  // OLED Display (same as before)
  display.clearDisplay();
  
  int tankX = 0, tankY = 5, tankW = 55, tankH = 55;
  display.drawRect(tankX, tankY, tankW, tankH, SSD1306_WHITE);
  
  int fillH = map(percentage, 0, 100, 0, tankH - 2);
  int fillY = tankY + tankH - 1 - fillH;
  display.fillRect(tankX + 1, fillY, tankW - 2, fillH, SSD1306_WHITE);

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(60, 30);
  display.print(percentage);
  display.print("%");

  display.setTextSize(1);
  display.setCursor(60, 10);
  display.println("Water Level");

  display.setCursor(60, 50);
  display.print(waterLevel);
  display.println(" cm");

  
  display.display();

  // MOTOR CONTROL 
  if (percentage >= 100) {
    motorState = true;   // Tank full → motor OFF
  } 
  else if (percentage <= 20) {
    motorState = false;    // Tank low → motor ON
  }
  // If between 20–99%, keep last state (so motor keeps running until full)

  if (RELAY_ACTIVE_HIGH) {
    digitalWrite(MOTOR_PIN, motorState ? HIGH : LOW);
  } else {
    digitalWrite(MOTOR_PIN, motorState ? LOW : HIGH);
  }

  // Buzzer and LED control (same as before)
  if (percentage <= 20) {
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }

  if (percentage <= 30) {
    digitalWrite(LED_MIN, HIGH);
    digitalWrite(LED_MID, LOW);
    digitalWrite(LED_MAX, LOW);
  } else if (percentage > 30 && percentage <= 70) {
    digitalWrite(LED_MIN, LOW);
    digitalWrite(LED_MID, HIGH);
    digitalWrite(LED_MAX, LOW);
  } else {
    digitalWrite(LED_MIN, LOW);
    digitalWrite(LED_MID, LOW);
    digitalWrite(LED_MAX, HIGH);
  }

  delay(500);
}