from flask import Flask, request
import requests

app = Flask(__name__)

@app.route("/")
def home():
    return "ESP32 Gateway Aktif"

@app.route("/webhook", methods=["GET"])
def webhook():
    msg = request.args.get("msg", "pesan kosong")
    send_telegram(msg)
    return f"Pesan dikirim: {msg}"

def send_telegram(msg):
    bot_token = "8175344760:AAFvyssP6vnamA5lqhwGSxsGzhmPuvNRzoM"
    chat_id = "1712293814"
    url = f"https://api.telegram.org/bot{bot_token}/sendMessage"
    requests.post(url, data={"chat_id": chat_id, "text": msg})

if __name__ == "__main__":
    app.run()
