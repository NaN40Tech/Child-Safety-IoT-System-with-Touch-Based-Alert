const express = require("express");
const axios = require("axios");

const app = express();
const PORT = process.env.PORT || 8080;

const TELEGRAM_BOT_TOKEN = "8175344760:AAFvyssP6vnamA5lqhwGSxsGzhmPuvNRzoM";
const TELEGRAM_CHAT_ID = "1712293814";

app.get("/send", async (req, res) => {
  const { bpm, lat, lon } = req.query;

  if (!bpm || !lat || !lon) {
    return res.status(400).send("Missing parameters");
  }

  const message = `🚨 *ALERT!*\nBPM: ${bpm}\nLokasi: https://maps.google.com/?q=${lat},${lon}`;
  const url = `https://api.telegram.org/bot${TELEGRAM_BOT_TOKEN}/sendMessage`;

  try {
    await axios.post(url, {
      chat_id: TELEGRAM_CHAT_ID,
      text: message,
      parse_mode: "Markdown"
    });
    res.send("Pesan berhasil dikirim ke Telegram!");
  } catch (err) {
    console.error(err.response?.data || err.message);
    res.status(500).send("Gagal mengirim pesan.");
  }
});

app.listen(PORT, () => {
  console.log(`Server berjalan di http://localhost:${PORT}`);
});
