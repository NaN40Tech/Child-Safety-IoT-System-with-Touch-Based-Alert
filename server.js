const http = require("http");
const express = require("express");
const RED = require("node-red");
require("dotenv").config(); // <-- load .env

const app = express();
const server = http.createServer(app);

// Configurasi dasar + inject env ke global
const settings = {
  httpAdminRoot: "/red",
  httpNodeRoot: "/api",
  userDir: "./.nodered",
  functionGlobalContext: {
    telegramToken: process.env.TELEGRAM_TOKEN,
    chatId: process.env.CHAT_ID,
    firebaseUrl: process.env.FIREBASE_URL,
  }
};

// Init & start Node-RED
RED.init(server, settings);
app.use(settings.httpAdminRoot, RED.httpAdmin);
app.use(settings.httpNodeRoot, RED.httpNode);

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
  console.log(`🚀 Server running on port ${PORT}`);
});

RED.start();
