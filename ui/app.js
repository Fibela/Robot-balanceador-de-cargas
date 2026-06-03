let port;
let reader;
let keepReading = false;
let writer;

const $ = (id) => document.getElementById(id);

const state = {
  sat: null,
  rms: null
};

function addLog(line) {
  const box = $("logBox");
  box.textContent += line + "\n";
  box.scrollTop = box.scrollHeight;
}

function setSuggestion() {
  let msg = "Sistema dentro de parámetros esperados.";
  if (state.sat !== null && state.sat > 30) {
    msg = "Sugerencia: saturación alta, reduce ganancia PID o limita rango mecánico.";
  } else if (state.rms !== null && state.rms > 2.0) {
    msg = "Sugerencia: RMS elevado, revisar estabilidad y ruido de sensor.";
  }
  $("suggestion").textContent = "Sugerencia: " + msg;
}

async function connectSerial() {
  if (!("serial" in navigator)) {
    alert("Tu navegador no soporta Web Serial API. Usa Chrome/Edge.");
    return;
  }
  port = await navigator.serial.requestPort();
  await port.open({ baudRate: 115200 });
  $("connState").textContent = "Estado: conectado";
  writer = port.writable.getWriter();
  keepReading = true;
  readLoop();
}

async function disconnectSerial() {
  keepReading = false;
  try { if (reader) await reader.cancel(); } catch (_) {}
  try { if (writer) writer.releaseLock(); } catch (_) {}
  try { if (port) await port.close(); } catch (_) {}
  $("connState").textContent = "Estado: desconectado";
}

async function sendLine(line) {
  if (!writer) return;
  const enc = new TextEncoder();
  await writer.write(enc.encode(line + "\n"));
  addLog(">> " + line);
}

function handleDataLine(line) {
  const parts = line.split(",");
  if (parts.length < 8) return;
  $("distance").textContent = parts[2];
  $("error").textContent = parts[3];
  $("servo").textContent = parts[4];
  $("latency").textContent = parts[5];
  $("weight").textContent = parts[6];
  $("mode").textContent = parts[7];
}

function handleMetricLine(line) {
  const satMatch = line.match(/sat_pct=([0-9.\-]+)/);
  const rmsMatch = line.match(/rms_cm=([0-9.\-]+)/);
  if (satMatch) {
    state.sat = parseFloat(satMatch[1]);
    $("sat").textContent = state.sat.toFixed(2);
  }
  if (rmsMatch) {
    state.rms = parseFloat(rmsMatch[1]);
    $("rms").textContent = state.rms.toFixed(3);
  }
  setSuggestion();
}

async function readLoop() {
  const dec = new TextDecoder();
  reader = port.readable.getReader();
  let buffer = "";
  while (keepReading) {
    const { value, done } = await reader.read();
    if (done) break;
    buffer += dec.decode(value, { stream: true });
    const lines = buffer.split("\n");
    buffer = lines.pop() || "";
    for (const raw of lines) {
      const line = raw.trim();
      if (!line) continue;
      addLog(line);
      if (line.startsWith("DATA,")) handleDataLine(line);
      if (line.startsWith("METRIC,")) handleMetricLine(line);
    }
  }
  reader.releaseLock();
}

$("btnConnect").addEventListener("click", connectSerial);
$("btnDisconnect").addEventListener("click", disconnectSerial);
$("btnAuto").addEventListener("click", () => sendLine("MODE,AUTO"));
$("btnManual").addEventListener("click", () => sendLine("MODE,MANUAL"));
$("btnStatus").addEventListener("click", () => sendLine("STATUS"));

$("servoSlider").addEventListener("input", (e) => {
  $("servoValue").textContent = e.target.value;
});

$("btnSendManual").addEventListener("click", () => {
  const v = $("servoSlider").value;
  sendLine(`MANUAL,${v}`);
});
