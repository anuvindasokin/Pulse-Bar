# PulseBar Connect

PulseBar Connect is the small cloud service that replaces a third-party social API gateway for PulseBar's Facebook and Instagram use case. It keeps Meta application secrets and Page tokens out of sellable ESP32 firmware.

## Deploy to Render

1. Create a Meta Business app and add Facebook Login plus the Instagram API products required for Page and Professional-account insights.
2. In Meta, add `https://YOUR-SERVICE.onrender.com/oauth/meta/callback` as an exact valid OAuth redirect URI.
3. Create a Render Blueprint from the repository's `render.yaml`.
4. Configure `PUBLIC_BASE_URL`, `META_APP_ID`, `META_APP_SECRET`, and `DEVICE_KEYS` in Render. Render generates the state and encryption secrets.
5. Configure Meta's privacy-policy URL as `https://YOUR-SERVICE.onrender.com/privacy` and data-deletion URL as `https://YOUR-SERVICE.onrender.com/data-deletion`. Replace these minimal endpoints with your final legal documents before selling the product.
6. In the PulseBar interface, open Facebook or Instagram, expand **Installer: configure PulseBar Connect server**, and enter the Render URL plus one device ID/secret pair from `DEVICE_KEYS`.

`DEVICE_KEYS` is a JSON object. Give every sold device a different randomly generated secret:

```json
{"PB-000001":"long-random-secret-for-this-device","PB-000002":"another-long-random-secret"}
```

## Local verification

Copy `.env.example` values into your shell environment, then run:

```sh
npm test
npm start
```

The OAuth connection becomes functional only after real Meta credentials and an HTTPS public callback domain are configured. Facebook and Instagram metrics also depend on Meta granting the requested permissions during App Review.
