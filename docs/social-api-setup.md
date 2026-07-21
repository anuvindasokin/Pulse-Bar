# Social API setup

PulseBar uses official platform APIs. A public profile URL identifies an account but does not authorize API requests. Enter credentials only in the local PulseBar interface; never commit them to Git.

## YouTube

1. Sign in to the [Google Cloud Console](https://console.cloud.google.com/) and create or select a project.
2. Open **APIs & Services → Library**, find **YouTube Data API v3**, and enable it.
3. Open **APIs & Services → Credentials**, then choose **Create credentials → API key**.
4. Restrict the key to YouTube Data API v3 where practical.
5. In PulseBar, enter the channel URL or `@handle` and API key.

Public channel subscriber and view statistics are available through `channels.list`. Public subscriber counts are rounded to three significant figures. See Google's [getting-started guide](https://developers.google.com/youtube/v3/getting-started) and [`channels.list` reference](https://developers.google.com/youtube/v3/docs/channels/list).

## Facebook and Instagram — one Meta connection

1. Use a Facebook Page that the customer manages and an Instagram Professional account (Business or Creator).
2. Link the Instagram Professional account to that Facebook Page.
3. Create one app in [Meta for Developers](https://developers.facebook.com/apps/) and configure the Facebook Login and Instagram API use cases required by PulseBar.
4. Authorize the connected accounts once and enter the resulting Meta access token in either PulseBar tab. PulseBar stores it as a shared Meta credential, so the customer does not enter it twice.
5. Add the Facebook Page URL and Instagram profile URL in their respective tabs.
6. Complete Meta App Review and switch the app to Live before serving customers outside its administrator/developer/tester roles.

Personal Facebook profiles and personal Instagram accounts are not supported. Follow the [official Meta access-token guide](https://developers.facebook.com/docs/facebook-login/guides/access-tokens/) and [Instagram API getting-started guide](https://developers.facebook.com/docs/instagram-platform/get-started/). Available metrics depend on account type, permissions, app mode, API version, and review approval.

## Security and production

- PulseBar never returns saved keys or tokens through its normal account API.
- Restrict API keys and request the minimum necessary permissions.
- Tokens can expire or be revoked, so the product must support reauthorization.
- A commercial multi-customer product normally needs approved Google/Meta applications, a privacy policy, a data-deletion process, and platform review.
