# Architecture

`AppController` owns cooperative, non-blocking services. Network and web requests are polled frequently; timing uses monotonic milliseconds; rendering is capped at 25 FPS. `MatrixMapper` is the only module translating logical 32×8 coordinates into physical LED indices, keeping wiring details out of scenes.

The current web UI is compiled into flash for a zero-step first boot. It talks to versioned JSON endpoints and polls live state once per second. A future filesystem asset pipeline can replace the embedded UI without changing the API.
