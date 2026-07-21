# Social API setup

PulseBar uses official platform APIs. A public profile URL identifies an account but does not authorize API requests. Enter credentials only in the local PulseBar interface; never commit them to Git.

## YouTube

1. Sign in to the [Google Cloud Console](https://console.cloud.google.com/) and create or select a project.
2. Open **APIs & Services → Library**, find **YouTube Data API v3**, and enable it.
3. Open **APIs & Services → Credentials**, then choose **Create credentials → API key**.
4. Restrict the key to YouTube Data API v3 where practical.
5. In PulseBar, enter the channel URL or `@handle` and API key.

Public channel subscriber and view statistics are available through `channels.list`. Public subscriber counts are rounded to three significant figures. See Google's [getting-started guide](https://developers.google.com/youtube/v3/getting-started) and [`channels.list` reference](https://developers.google.com/youtube/v3/docs/channels/list).

## Facebook

1. Use a Facebook Page that the customer manages. Personal profiles are not Pages.
2. Create an app in [Meta for Developers](https://developers.facebook.com/apps/).
3. Add the appropriate Facebook Login/Pages use case and connect the Page.
4. Follow Meta's current flow to generate a Page access token with the permissions required by the Pages endpoints PulseBar uses.
5. Complete Meta App Review and switch the app to Live before serving customers outside its administrator/developer/tester roles.
6. Enter the Page URL and Page access token in PulseBar.

Follow the [official Meta access-token guide](https://developers.facebook.com/docs/facebook-login/guides/access-tokens/). Available metrics depend on Page access, app mode, API version, permissions, and review approval.

## Instagram

1. Use an Instagram Professional account (Business or Creator). Personal accounts do not expose the same insights APIs.
2. Connect the account to a Facebook Page when required by Meta's current setup flow.
3. Create a Meta developer app and add the Instagram API use case.
4. Follow Meta's current authorization flow and request only the permissions required for profile and insights data.
5. Complete App Review before authorizing customers outside the app's administrator/developer/tester roles.
6. Enter the profile URL and access token in PulseBar.

Follow Meta's [official Instagram API getting-started guide](https://developers.facebook.com/docs/instagram-platform/get-started/). Metric availability varies by account and media type; PulseBar must report unsupported metrics rather than inventing values.

## Security and production

- PulseBar never returns saved keys or tokens through its normal account API.
- Restrict API keys and request the minimum necessary permissions.
- Tokens can expire or be revoked, so the product must support reauthorization.
- A commercial multi-customer product normally needs approved Google/Meta applications, a privacy policy, a data-deletion process, and platform review.
