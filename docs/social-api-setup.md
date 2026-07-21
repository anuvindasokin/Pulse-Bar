# Social API setup

PulseBar uses official platform APIs. A public profile URL identifies an account but does not authorize API requests. Enter credentials only in the local PulseBar interface; never commit them to Git.

## YouTube

1. Sign in to the [Google Cloud Console](https://console.cloud.google.com/) and create or select a project.
2. Open **APIs & Services → Library**, find **YouTube Data API v3**, and enable it.
3. Open **APIs & Services → Credentials**, then choose **Create credentials → API key**.
4. Restrict the key to YouTube Data API v3 where practical.
5. In PulseBar, enter the channel URL or `@handle` and API key.

Public channel subscriber and view statistics are available through `channels.list`. Public subscriber counts are rounded to three significant figures. See Google's [getting-started guide](https://developers.google.com/youtube/v3/getting-started) and [`channels.list` reference](https://developers.google.com/youtube/v3/docs/channels/list).

## Facebook and Instagram — PulseBar Connect

Customers no longer paste Meta tokens into PulseBar. The seller deploys the included `cloud/` service once, configures it in the device's installer section, and the customer presses **Connect Facebook & Instagram**.

1. Deploy PulseBar Connect using `render.yaml` and follow `cloud/README.md`.
2. Create one app in [Meta for Developers](https://developers.facebook.com/apps/) and configure its OAuth callback, privacy policy, data-deletion URL, Facebook Login, and Instagram API permissions.
3. Complete Meta App Review and switch the app to Live before serving customers outside its administrator/developer/tester roles.
4. Provision every manufactured PulseBar with `tools/provision_device.py`; this creates its database record, unique secret, and one-time claim code.
5. Put the device ID/secret in the installer panel and give the customer only the ID/claim code.
6. The customer creates a PulseBar account, claims one or more devices, authorizes Meta, and selects the correct Page. PulseBar Connect encrypts the Page token at rest and supplies normalized counters only to the authenticated device.

Personal Facebook profiles and personal Instagram accounts are not supported. Follow the [official Meta access-token guide](https://developers.facebook.com/docs/facebook-login/guides/access-tokens/) and [Instagram API getting-started guide](https://developers.facebook.com/docs/instagram-platform/get-started/). Available metrics depend on account type, permissions, app mode, API version, and review approval.

## Security and production

- Meta app secrets and access tokens stay in PulseBar Connect and are never returned to the device UI.
- Restrict API keys and request the minimum necessary permissions.
- Tokens can expire or be revoked, so the product must support reauthorization.
- A commercial multi-customer product normally needs approved Google/Meta applications, a privacy policy, a data-deletion process, and platform review.
