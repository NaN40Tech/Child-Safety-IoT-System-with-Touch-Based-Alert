const express = require("express");
const TelegramBot = require("node-telegram-bot-api");

const app = express();
const PORT = process.env.PORT || 8080;

// Token bot Telegram kamu
const TELEGRAM_TOKEN = "8175344760:AAFvyssP6vnamA5lqhwGSxsGzhmPuvNRzoM";
const CHAT_ID = "1712293814"; // ganti dengan ID Telegram kamu

const bot = new TelegramBot(TELEGRAM_TOKEN);

app.get("/send", (req, res) => {
  const { bpm, lat, lon } = req.query;

  if (!bpm || !lat || !lon) {
    return res.status(400).send("Missing parameters");
  }

  const message = `‼️ *ALERT*\nBPM: ${bpm}\nLokasi: https://maps.google.com/?q=${lat},${lon}`;
  bot.sendMessage(CHAT_ID, message, { parse_mode: "Markdown" });

  console.log(`[RECEIVED] BPM: ${bpm}, Lokasi: ${lat}, ${lon}`);
  res.send("OK");
});

app.get("/", (req, res) => {
  res.send("Server IoT Anak aktif!");
});

app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});
