from flask import Flask, request, render_template_string

app = Flask(__name__)

# Store data
data = {"temperature": "--", "humidity": "--"}

html = """
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Sensor Data</title>
    <meta http-equiv="refresh" content="5">
    <style>
        body {
            background-color: #e8f5e9; /* light green */
            color: #00ff00; /* neon green text */
            font-family: sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }

        .box {
            background-color: black;
            color: #00ff00;
            padding: 20px;
            border-radius: 10px;
            width: 300px;
            text-align: center;
            box-shadow: 0 0 15px #00ff00;
        }

        h1 {
            font-size: 24px;
            margin-bottom: 15px;
        }

        p {
            font-size: 18px;
            margin: 10px 0;
        }
    </style>
</head>
<body>
    <div class="box">
        <h1>🌡️ ESP32 Sensor Data</h1>
        <p>Temperature: {{ temp }} &deg;C</p>
        <p>Humidity: {{ hum }} %</p>
    </div>
</body>
</html>
"""

@app.route("/")
def index():
    return render_template_string(html, temp=data["temperature"], hum=data["humidity"])

@app.route("/data", methods=["POST"])
def receive_data():
    data["temperature"] = request.form.get("temperature", "--")
    data["humidity"] = request.form.get("humidity", "--")
    print(f"Received -> Temp: {data['temperature']}, Humidity: {data['humidity']}")
    return "OK", 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)

