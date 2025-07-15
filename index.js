const express = require('express');
const axios = require('axios');
const app = express();

const TELEGRAM_BOT_TOKEN = '8175344760:AAFvyssP6vnamA5lqhwGSxsGzhmPuvNRzoM';
const TELEGRAM_CHAT_ID = '1712293814';

app.get('/send', async (req, res) => {
  const { bpm, lat, lon } = req.query;
  if (!bpm || !lat || !lon) {
    return res.status(400).send('Missing parameters');
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
  } catch (err) {
    res.status(500).send('Telegram error');
  }
});

app.get('/', (req, res) => {
  res.send('Child Tracker is running');
});

const port = process.env.PORT || 3000;
app.listen(port, () => console.log(`Server running on port ${port}`));
