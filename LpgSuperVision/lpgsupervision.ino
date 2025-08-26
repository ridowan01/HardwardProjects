#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "LPG Projection Lab";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

#define POWER_PIN  D7 // power giving
#define SIGNAL_PIN A0 // analog reading
#define BLUE D0 // blue led pin
#define YELLOW D1 // yellow led pin
#define RED D4 // red led pin
#define BUZZER D2 //buzzer pin

int value = 0; // variable to store the sensor value
int used = 0; // store used ammount
int remining = 0; // store remining amound

void setup() {
  Serial.begin(115200); // starting serial monitor

  pinMode(POWER_PIN, OUTPUT);   // Configure D7 pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW); // turn the sensor OFF

  // hosting local netword
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  // publishing website
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  
  server.begin(); // Starting server
  Serial.println("HTTP server started");

  // setiing pins
  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // sensor reading
  digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds

  // sensor reading and calculation
  value = analogRead(SIGNAL_PIN); // read the analog value from sensor
  remining = map(value, 0, 660, 0, 100);
  used = 100-remining;
  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF

  Serial.print(value);
  Serial.print(" ");
  Serial.print(remining);
  Serial.print(" ");
  Serial.println(used);

  color(used);

  // web publishing
  server.handleClient();
}

void handle_OnConnect() { // publishing website
  Serial.println("Server Running");
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){ // handeling connections
  server.send(404, "text/plain", "Not found");
}

void color(int usedValue) {
  if (usedValue < 0) usedValue = 0;
  if (usedValue > 100) usedValue = 100;

  int remaining = 100 - usedValue;

  if (remaining > 30) {
    digitalWrite(BLUE, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    noTone(BUZZER);
  }
  else if (remaining < 30 && remaining >= 10) {
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    noTone(BUZZER); 
  }
  else {
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, HIGH);

    // buzzer beeps
    tone(BUZZER, 3000);
  }
}

String SendHTML(){  // the webpage code
  // html file
  String html = "<!DOCTYPE html>\n";
  html += "<html lang=\"en\"> \n";
  html += " \n";
  html += "<head> \n";
  html += "    <meta charset=\"UTF-8\" /> \n";
  html += "    <title>LPG Level Detector</title> \n";
  html += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" /> \n";
  html += "    <style> \n";
  html += "        body { \n";
  html += "            margin: 0; \n";
  html += "            padding: 0; \n";
  html += "            background-color: #e0f7fa; \n";
  html += "            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; \n";
  html += "            font-weight: bold; \n";
  html += "            color: #000; \n";
  html += "        } \n";
  html += " \n";
  html += "        #header { \n";
  html += "            background: linear-gradient(135deg, #0f2027, #203a43, #2c5364); \n";
  html += "            color: white; \n";
  html += "            padding: 40px 0; \n";
  html += "            text-align: center; \n";
  html += "            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.3); \n";
  html += "        } \n";
  html += " \n";
  html += "        #header h1 { \n";
  html += "            margin: 0; \n";
  html += "            font-size: 42px; \n";
  html += "            font-weight: 700; \n";
  html += "            letter-spacing: 2px; \n";
  html += "        } \n";
  html += " \n";
  html += "        #header p { \n";
  html += "            margin-top: 10px; \n";
  html += "            font-size: 18px; \n";
  html += "            font-weight: 300; \n";
  html += "            color: #ccc; \n";
  html += "        } \n";
  html += " \n";
  html += "        #mainContainer { \n";
  html += "            max-width: 1000px; \n";
  html += "            background-color: white; \n";
  html += "            margin: 40px auto; \n";
  html += "            padding: 35px 30px; \n";
  html += "            border-radius: 15px; \n";
  html += "            box-shadow: 0 0 15px rgba(0, 0, 0, 0.2); \n";
  html += "        } \n";
  html += " \n";
  html += "        #statusBox { \n";
  html += "            margin-top: 35px; \n";
  html += "            padding: 18px; \n";
  html += "            background-color: #e0f2f1; \n";
  html += "            border-radius: 10px; \n";
  html += "            text-align: center; \n";
  html += "            color: #00796b; \n";
  html += "            box-shadow: 0 0 8px rgba(0, 0, 0, 0.08); \n";
  html += "        } \n";
  html += " \n";
  html += "        #contentFlex { \n";
  html += "            display: flex; \n";
  html += "            flex-wrap: wrap; \n";
  html += "            gap: 20px; \n";
  html += "        } \n";
  html += " \n";
  html += "        #barsContainer { \n";
  html += "            flex: 1; \n";
  html += "            min-width: 250px; \n";
  html += "        } \n";
  html += " \n";
  html += "        #graphContainer { \n";
  html += "            flex: 1; \n";
  html += "            min-width: 250px; \n";
  html += "            height: 350px; \n";
  html += "        } \n";
  html += " \n";
  html += "        footer { \n";
  html += "            text-align: center; \n";
  html += "            padding: 15px; \n";
  html += "            background-color: #263238; \n";
  html += "            color: white; \n";
  html += "            font-size: 14px; \n";
  html += "        } \n";
  html += "    </style> \n";
  html += "</head> \n";
  html += " \n";
  html += "<body> \n";
  html += " \n";
  html += "    <div id=\"header\"> \n";
  html += "        <h1>LPG LEVEL DETECTOR</h1> \n";
  html += "        <p>Real-Time Gas Monitoring Dashboard</p> \n";
  html += "    </div> \n";
  html += " \n";
  html += "    <div id=\"mainContainer\"> \n";
  html += "        <div id=\"contentFlex\"> \n";
  html += "            <div id=\"barsContainer\"> \n";
  html += "                <p style=\"font-size: 20px;\">Used Amount: <span id=\"usedVal\">0%</span></p> \n";
  html += "                <div style=\"background-color: #ffebee; border-radius: 6px; overflow: hidden; height: 30px;\"> \n";
  html += "                    <div id=\"usedBar\" \n";
  html += "                        style=\"background-color: #d32f2f; height: 100%; width: 0%; transition: width 0.5s;\"></div> \n";
  html += "                </div> \n";
  html += " \n";
  html += "                <p style=\"font-size: 20px; margin-top: 30px;\">Remaining Amount: <span id=\"remVal\">100%</span></p> \n";
  html += "                <div style=\"background-color: #e8f5e9; border-radius: 6px; overflow: hidden; height: 30px;\"> \n";
  html += "                    <div id=\"remBar\" \n";
  html += "                        style=\"background-color: #388e3c; height: 100%; width: 100%; transition: width 0.5s;\"></div> \n";
  html += "                </div> \n";
  html += " \n";
  html += "                <div id=\"statusBox\"> \n";
  html += "                    <p id=\"statusText\" style=\"font-size: 18px;\">STATUS: System Normal</p> \n";
  html += "                </div> \n";
  html += "            </div> \n";
  html += " \n";
  html += "            <div id=\"graphContainer\"> \n";
  html += "                <canvas id=\"usageChart\" width=\"400\" height=\"350\"></canvas> \n";
  html += "            </div> \n";
  html += "        </div> \n";
  html += "    </div> \n";
  html += " \n";
  html += "    <footer> \n";
  html += "        Powered by: LPG Projection Lab<br> \n";
  html += "        @ RoLL: 2303103 - 108 \n";
  html += "    </footer> \n";
  html += " \n";
  html += "    <script> \n";
  html += "        function drawChart(labels, data) { \n";
  html += "            const canvas = document.getElementById('usageChart'); \n";
  html += "            const ctx = canvas.getContext('2d'); \n";
  html += "            ctx.clearRect(0, 0, canvas.width, canvas.height); \n";
  html += " \n";
  html += "            const padding = 50; \n";
  html += "            const chartHeight = canvas.height - padding * 2; \n";
  html += "            const chartWidth = canvas.width - padding * 2; \n";
  html += "            const maxVal = 100; \n";
  html += "            const stepY = 20; \n";
  html += " \n";
  html += "            // Draw axes \n";
  html += "            ctx.strokeStyle = '#000'; \n";
  html += "            ctx.lineWidth = 1; \n";
  html += "            ctx.beginPath(); \n";
  html += "            ctx.moveTo(padding, padding); \n";
  html += "            ctx.lineTo(padding, canvas.height - padding); \n";
  html += "            ctx.lineTo(canvas.width - padding, canvas.height - padding); \n";
  html += "            ctx.stroke(); \n";
  html += " \n";
  html += "            // Y-axis ticks and labels \n";
  html += "            ctx.fillStyle = '#000'; \n";
  html += "            ctx.textAlign = 'right'; \n";
  html += "            ctx.textBaseline = 'middle'; \n";
  html += "            for (let i = 0; i <= maxVal; i += stepY) { \n";
  html += "                const y = canvas.height - padding - (i / maxVal) * chartHeight; \n";
  html += "                ctx.beginPath(); \n";
  html += "                ctx.moveTo(padding - 5, y); \n";
  html += "                ctx.lineTo(padding, y); \n";
  html += "                ctx.stroke(); \n";
  html += "                ctx.fillText(i + '%', padding - 10, y); \n";
  html += "            } \n";
  html += " \n";
  html += "            // X-axis ticks and labels (dynamic) \n";
  html += "            const stepX = chartWidth / (labels.length - 1); \n";
  html += "            ctx.textAlign = 'center'; \n";
  html += "            ctx.textBaseline = 'top'; \n";
  html += "            for (let i = 0; i < labels.length; i++) { \n";
  html += "                const x = padding + i * stepX; \n";
  html += "                ctx.beginPath(); \n";
  html += "                ctx.moveTo(x, canvas.height - padding); \n";
  html += "                ctx.lineTo(x, canvas.height - padding + 5); \n";
  html += "                ctx.stroke(); \n";
  html += "                ctx.fillText(labels[i], x, canvas.height - padding + 8); \n";
  html += "            } \n";
  html += " \n";
  html += "            // Plot line \n";
  html += "            ctx.beginPath(); \n";
  html += "            ctx.strokeStyle = '#0077cc'; \n";
  html += "            ctx.lineWidth = 2; \n";
  html += "            data.forEach((val, i) => { \n";
  html += "                const x = padding + i * stepX; \n";
  html += "                const y = canvas.height - padding - (val / maxVal) * chartHeight; \n";
  html += "                if (i === 0) ctx.moveTo(x, y); \n";
  html += "                else ctx.lineTo(x, y); \n";
  html += "            }); \n";
  html += "            ctx.stroke(); \n";
  html += "        } \n";
  html += " \n";
  html += "        function generateProjection(usedValue) { \n";
  html += "            const remainingStart = 100 - usedValue; \n";
  html += "            const totalDays = usedValue >= 95 ? 0.1 : usedValue >= 90 ? 2 : usedValue >= 80 ? 7 : usedValue >= 70 ? 15 : usedValue >= 60 ? 30 : usedValue >= 30 ? 40 : 50; \n";
  html += " \n";
  html += "            const labels = []; \n";
  html += "            const remainingValues = []; \n";
  html += "            let current = remainingStart; \n";
  html += " \n";
  html += "            const interval = Math.ceil(totalDays / 5); \n";
  html += "            for (let i = 0; i <= totalDays; i += interval) { \n";
  html += "                labels.push(`Day ${i}`); \n";
  html += "                remainingValues.push(parseFloat(current.toFixed(1))); \n";
  html += " \n";
  html += "                const step = remainingStart / (totalDays / interval); \n";
  html += "                const variation = step * (0.8 + Math.random() * 0.4); \n";
  html += "                current = Math.max(0, current - variation); \n";
  html += "            } \n";
  html += " \n";
  html += "            return { labels, remainingValues }; \n";
  html += "        } \n";
  html += " \n";
  html += " \n";
  html += "        function setLPGLevel(usedValue) { \n";
  html += "            usedValue = Math.max(0, Math.min(100, usedValue)); \n";
  html += "            const remaining = 100 - usedValue; \n";
  html += " \n";
  html += "            document.getElementById(\"usedVal\").innerText = usedValue + \"%\"; \n";
  html += "            document.getElementById(\"remVal\").innerText = remaining + \"%\"; \n";
  html += "            document.getElementById(\"usedBar\").style.width = usedValue + \"%\"; \n";
  html += "            document.getElementById(\"remBar\").style.width = remaining + \"%\"; \n";
  html += " \n";
  html += "            const statusBox = document.getElementById(\"statusBox\"); \n";
  html += "            const statusText = document.getElementById(\"statusText\"); \n";
  html += "            let status = \"STATUS: System Normal\"; \n";
  html += "            let bgColor = \"#e0f2f1\"; \n";
  html += "            let textColor = \"#00796b\"; \n";
  html += " \n";
  html += "            if (remaining < 30 && remaining >= 10) { \n";
  html += "                status = \"STATUS: LPG Getting Low\"; \n";
  html += "                bgColor = \"#fff8e1\"; \n";
  html += "                textColor = \"#f57c00\"; \n";
  html += "            } \n";
  html += "            if (remaining < 10 && remaining > 0) { \n";
  html += "                status = \"STATUS: Critical! Refill Required\"; \n";
  html += "                bgColor = \"#ffebee\"; \n";
  html += "                textColor = \"#c62828\"; \n";
  html += "            } \n";
  html += "            if (remaining === 0) { \n";
  html += "                status = \"STATUS: LPG Finished! Immediate Action Needed!\"; \n";
  html += "                bgColor = \"#ffccbc\"; \n";
  html += "                textColor = \"#b71c1c\"; \n";
  html += "            } \n";
  html += " \n";
  html += "            statusBox.style.backgroundColor = bgColor; \n";
  html += "            statusText.innerText = status; \n";
  html += "            statusText.style.color = textColor; \n";
  html += " \n";
  html += "            const projection = generateProjection(usedValue); \n";
  html += "            drawChart(projection.labels, projection.remainingValues); \n";
  html += "        } \n";
  html += " \n";
  html += "        setLPGLevel(" + String(used) + "); // Example initialization \n";
  html += "    </script> \n";
  html += "</body> \n";
  html += " \n";
  html += "</html> \n";
  return html;
}