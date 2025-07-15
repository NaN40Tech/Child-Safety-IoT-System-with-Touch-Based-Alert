const express = require('express');
const axios = require('axios');
const app = express();

const CHAT_ID = '1712293814';
const BOT_TOKEN = '8175344760:AAFvyssP6vnamA5lqhwGSxsGzhmPuvNRzoM';

app.get('/send', async (req, res) => {
  const { bpm, lat, lon } = req.query;

  if (!bpm || !lat || !lon) {
    return res.status(400).send('Missing parameters');
  }

  const text = `🚨 *ALERT!*\nBPM: ${bpm}\nLokasi: https://maps.google.com/?q=${lat},${lon}`;
  const url = `https://api.telegram.org/bot${BOT_TOKEN}/sendMessage`;

  try {
    const response = await axios.get(url, {
      params: {
        chat_id: CHAT_ID,
        text: text,
        parse_mode: 'Markdown'
      }
    });

    res.status(200).send('Pesan terkirim ke Telegram');
  } catch (err) {
    res.status(500).send('Gagal kirim ke Telegram');
  }
});

const PORT = process.env.PORT || 8080;
app.listen(PORT, () => {
  console.log(`Server listening on port ${PORT}`);
});
