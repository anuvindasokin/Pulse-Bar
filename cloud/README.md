# PulseBar Connect

PulseBar Connect is the multi-customer cloud service for PulseBar's Facebook and Instagram counters. It provides customer accounts, device ownership/claiming, Meta OAuth, Page selection, encrypted social tokens, and authenticated device metrics. Meta application secrets and Page tokens never enter sellable ESP32 firmware.

## Deploy to Render

1. Create a Meta Business app and add Facebook Login plus the Instagram API products required for Page and Professional-account insights.
2. In Meta, add `https://YOUR-SERVICE.onrender.com/oauth/meta/callback` as an exact valid OAuth redirect URI.
3. Create a Render Blueprint from the repository's `render.yaml`.
4. Review the paid Starter web service and PostgreSQL database shown by Render, then deploy.
5. Configure `PUBLIC_BASE_URL`, `META_APP_ID`, `META_APP_SECRET`, and a long random `ADMIN_TOKEN` in Render. Render creates `DATABASE_URL` and generates the state/encryption secrets.
6. Configure Meta's privacy-policy URL as `https://YOUR-SERVICE.onrender.com/privacy` and data-deletion URL as `https://YOUR-SERVICE.onrender.com/data-deletion`. Replace these minimal endpoints with final, lawyer-reviewed documents before selling.

## Provision every manufactured device

Create the device in PostgreSQL through the protected manufacturing endpoint:

```powershell
python tools/provision_device.py `
  --backend https://YOUR-SERVICE.onrender.com `
  --admin-token YOUR_ADMIN_TOKEN `
  --device-id PB-000001
```

The command returns three values once:

- **Device ID** and **device secret** go into the PulseBar installer settings during manufacturing.
- **Device ID** and **claim code** go on the customer card or QR label.
- The administrator token and device secret must never be printed for the customer or committed to Git.

## Customer flow

1. Customer connects PulseBar to Wi-Fi and presses **Connect Facebook & Instagram**.
2. The hosted portal opens. The customer creates an account or logs in.
3. Customer enters the one-time claim code included with the device.
4. Customer presses Connect, authorizes Meta, and selects the correct Facebook Page.
5. PulseBar polls authenticated metrics every five minutes.

One customer may own multiple devices. A claim code can be used only once. Passwords and device secrets are scrypt-hashed; sessions are stored server-side; social tokens are AES-256-GCM encrypted.

## Local verification

Run PostgreSQL locally, copy `.env.example` values into your shell environment, then run:

```sh
npm test
npm start
```

The OAuth connection becomes functional only after real Meta credentials and an HTTPS public callback domain are configured. Ordinary customers require Meta App Review for the requested permissions.
