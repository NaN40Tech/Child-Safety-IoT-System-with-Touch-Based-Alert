from flask import Flask, request
import os

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def handle_message():
    # Ambil parameter dari SIM800L (misalnya dari URL: ?msg=Halo)
    msg = request.args.get('msg') or 'Tidak ada pesan'
    print(f"Pesan diterima: {msg}")
    return f"Pesan diterima: {msg}", 200

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    app.run(host="0.0.0.0", port=port)
