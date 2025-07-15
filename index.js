const express = require('express');
const axios = require('axios');
const app = express();

const TELEGRAM_BOT_TOKEN = process.env.TELEGRAM_BOT_TOKEN;
const TELEGRAM_CHAT_ID = process.env.TELEGRAM_CHAT_ID;

app.get('/send', async (req, res) => {
  const { bpm, lat, lon } = req.query;
  if (!bpm || !lat || !lon) {
    return res.status(400).send('Missing bpm, lat, or lon');
  }

  const text = `🚨 ALERT!\nBPM: ${bpm}\nLatitude: ${lat}\nLongitude: ${lon}`;
  try {
    await axios.get(`https://api.telegram.org/bot${TELEGRAM_BOT_TOKEN}/sendMessage`, {
      params: {
        chat_id: TELEGRAM_CHAT_ID,
        text: text
      }
    });
    res.send('ok');
  } catch (error) {
    res.status(500).send('Failed to send message to Telegram');
  }
});

const PORT = process.env.PORT || 8080;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));
