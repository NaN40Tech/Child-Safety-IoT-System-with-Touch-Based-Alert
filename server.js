const express = require("express");
const TelegramBot = require("node-telegram-bot-api");

const app = express();
const PORT = process.env.PORT || 3000;

const TELEGRAM_TOKEN = "8175344760:AAFvyssP6vnamA5lqhwGSxsGzhmPuvNRzoM";
const CHAT_ID = "1712293814";

const bot = new TelegramBot(TELEGRAM_TOKEN);

app.get("/iot", (req, res) => {
  const { bpm, lat, lon } = req.query;
  if (!bpm || !lat || !lon) return res.status(400).send("Missing parameters");

  const msg = `‼️ *ALERT*\nBPM: ${bpm}\n📍 https://maps.google.com/?q=${lat},${lon}`;
  bot.sendMessage(CHAT_ID, msg, { parse_mode: "Markdown" });

  res.send("OK");
});

app.get("/", (req, res) => {
  res.send("Server aktif. Endpoint: /iot");
});

app.listen(PORT, () => {
  console.log(`✅ Server aktif di http://localhost:${PORT}`);
});
