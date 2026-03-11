// ─── URL del backend Flask ────────────────────────────────────────────────────
// Cambia esta línea con la IP/URL de tu servidor Flask
// Ejemplos:
//   Local:  "http://192.168.1.10:5000"
//   Render: "https://tu-app.onrender.com"
const DEFAULT_API = "http://172.22.49.14:5000";

// ─── Persistencia de la URL en localStorage ───────────────────────────────────
let API_BASE = localStorage.getItem("led_api_base") || DEFAULT_API;

// ─── Elementos DOM ────────────────────────────────────────────────────────────
const elColor      = document.getElementById("color");
const elHex        = document.getElementById("colorHex");
const elCount      = document.getElementById("count");
const elCountLabel = document.getElementById("countLabel");
const elStrip      = document.getElementById("strip");
const elBadge      = document.getElementById("netBadge");
const elRev        = document.getElementById("rev");
const elUpdatedAt  = document.getElementById("updatedAt");
const elApiInput   = document.getElementById("apiInput");
const elApiDisplay = document.getElementById("apiDisplay");
const elEsp32Url   = document.getElementById("esp32Url");
const btnSetApi    = document.getElementById("btnSetApi");
const btnOff       = document.getElementById("btnOff");
const btnFull      = document.getElementById("btnFull");
const btnSave      = document.getElementById("btnSave");

// ─── Estado local ─────────────────────────────────────────────────────────────
let current   = { color: "#ff0000", count: 0, rev: null, updated_at: null };
let saveTimer = null;

// ─── Init UI de URL ───────────────────────────────────────────────────────────
function refreshApiUi() {
  elApiInput.value      = API_BASE;
  elApiDisplay.textContent = `${API_BASE}/api/state`;
  elEsp32Url.textContent   = `${API_BASE}/api/state`;
}
refreshApiUi();

// ─── Utilidades ───────────────────────────────────────────────────────────────
const clamp = (n, a, b) => Math.max(a, Math.min(b, n));

function renderPreview() {
  elHex.textContent        = current.color.toLowerCase();
  elCountLabel.textContent = String(current.count);

  elStrip.innerHTML = "";
  for (let i = 0; i < 8; i++) {
    const d = document.createElement("div");
    d.className = "led";
    if (i < current.count) {
      d.style.background = current.color;
      d.style.boxShadow  = `0 0 10px 2px ${current.color}88`;
    }
    elStrip.appendChild(d);
  }

  elRev.textContent       = current.rev ?? "—";
  elUpdatedAt.textContent = current.updated_at
    ? new Date(current.updated_at).toLocaleString()
    : "—";
}

function setBadge(ok, text) {
  elBadge.textContent = text;
  elBadge.style.borderColor = ok
    ? "rgba(100,255,180,.35)"
    : "rgba(255,120,120,.35)";
  elBadge.style.background = ok
    ? "rgba(100,255,180,.12)"
    : "rgba(255,120,120,.12)";
}

// ─── API calls ────────────────────────────────────────────────────────────────
async function loadState() {
  try {
    const r  = await fetch(`${API_BASE}/api/state`, { cache: "no-store" });
    if (!r.ok) throw new Error(`HTTP ${r.status}`);
    const st = await r.json();

    current.color      = st.color;
    current.count      = clamp(parseInt(st.count, 10) || 0, 0, 8);
    current.rev        = st.rev ?? null;
    current.updated_at = st.updated_at ?? null;

    elColor.value = current.color;
    elCount.value = current.count;

    renderPreview();
    setBadge(true, "✅ Conectado");
  } catch {
    setBadge(false, "❌ Sin conexión");
  }
}

async function saveState() {
  try {
    const r = await fetch(`${API_BASE}/api/state`, {
      method:  "POST",
      headers: { "Content-Type": "application/json" },
      body:    JSON.stringify({ color: current.color, count: current.count }),
    });
    const out = await r.json();
    if (!r.ok) throw new Error(out?.error || `HTTP ${r.status}`);

    current.rev        = out.rev ?? current.rev;
    current.updated_at = out.updated_at ?? current.updated_at;

    renderPreview();
    setBadge(true, "💾 Guardado");
    setTimeout(() => setBadge(true, "✅ Conectado"), 1500);
  } catch (e) {
    setBadge(false, "❌ Error al guardar");
    console.error(e);
  }
}

function scheduleSave(ms = 180) {
  clearTimeout(saveTimer);
  saveTimer = setTimeout(saveState, ms);
}

// ─── Eventos ──────────────────────────────────────────────────────────────────

// Cambiar URL del backend desde la UI
btnSetApi.addEventListener("click", () => {
  const val = elApiInput.value.trim().replace(/\/$/, ""); // quita slash final
  if (!val) return;
  API_BASE = val;
  localStorage.setItem("led_api_base", API_BASE);
  refreshApiUi();
  loadState();
});

elApiInput.addEventListener("keydown", e => {
  if (e.key === "Enter") btnSetApi.click();
});

elColor.addEventListener("input", () => {
  current.color = elColor.value;
  renderPreview();
  scheduleSave();
});

elCount.addEventListener("input", () => {
  current.count = clamp(parseInt(elCount.value, 10) || 0, 0, 8);
  renderPreview();
  scheduleSave();
});

btnOff.addEventListener("click", () => {
  current.count = 0;
  elCount.value = 0;
  renderPreview();
  saveState();
});

btnFull.addEventListener("click", () => {
  current.count = 8;
  elCount.value = 8;
  renderPreview();
  saveState();
});

btnSave.addEventListener("click", saveState);

document.querySelectorAll(".preset").forEach(btn => {
  btn.addEventListener("click", () => {
    current.color = btn.dataset.color;
    elColor.value = current.color;
    renderPreview();
    saveState();
  });
});

// ─── Init ─────────────────────────────────────────────────────────────────────
renderPreview();
loadState();
