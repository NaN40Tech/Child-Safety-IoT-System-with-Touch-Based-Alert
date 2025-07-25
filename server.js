const http = require("http");
const express = require("express");
const RED = require("node-red");

const app = express();
const server = http.createServer(app);

// Konfigurasi dasar
const settings = {
  httpAdminRoot: "/red",
  httpNodeRoot: "/api",
  userDir: "./.nodered",
  functionGlobalContext: {}
};

// Init Node-RED
RED.init(server, settings);

// Route Admin UI & API
app.use(settings.httpAdminRoot, RED.httpAdmin);
app.use(settings.httpNodeRoot, RED.httpNode);

// Jalankan server
const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
  console.log(`🚀 Server running on port ${PORT}`);
});

// Start Node-RED
RED.start();
