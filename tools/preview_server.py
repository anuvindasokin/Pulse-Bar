#!/usr/bin/env python3
"""Desktop simulator for the exact web interface embedded in PulseBar firmware."""

import argparse
import json
import re
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
WEB_HEADER = ROOT / "src" / "web" / "WebUi.h"
STARTED_AT = time.monotonic()

state = {
    "name": "My PulseBar",
    "version": "0.1.0-desktop",
    "wifi": True,
    "setupRequired": False,
    "mode": "desktop preview",
    "ssid": "Home Wi-Fi",
    "ip": "127.0.0.1",
    "rssi": -48,
    "timezone": "IST-5:30",
    "brightness": 48,
    "color": "#FF4D91",
    "stopwatch": {"running": False, "elapsedMs": 0},
    "timer": {"running": False, "remainingMs": 0},
}
stopwatch_started = None
timer_started = None
accounts = {
    "youtube": {"url": "", "connected": False, "status": "Not connected"},
    "facebook": {"url": "", "connected": False, "status": "Not connected"},
    "instagram": {"url": "", "connected": False, "status": "Not connected"},
}
playlist = {"scenes": [
    {"id": "youtubeSubscribers", "enabled": True}, {"id": "youtubeViews", "enabled": True},
    {"id": "facebookFollowers", "enabled": True}, {"id": "facebookViews", "enabled": True},
    {"id": "instagramFollowers", "enabled": True}, {"id": "instagramViews", "enabled": True},
    {"id": "time", "enabled": True},
]}


def load_firmware_ui():
    """Read the C++ raw string each request so refresh always reflects firmware UI."""
    source = WEB_HEADER.read_text(encoding="utf-8")
    match = re.search(r'R"HTML\((.*)\)HTML";', source, re.DOTALL)
    if not match:
        raise RuntimeError(f"Could not locate WEB_UI raw string in {WEB_HEADER}")
    return match.group(1).encode("utf-8")


def current_status():
    global stopwatch_started, timer_started
    result = json.loads(json.dumps(state))
    result["uptime"] = int(time.monotonic() - STARTED_AT)
    if state["stopwatch"]["running"] and stopwatch_started is not None:
        result["stopwatch"]["elapsedMs"] += int((time.monotonic() - stopwatch_started) * 1000)
    if state["timer"]["running"] and timer_started is not None:
        elapsed = int((time.monotonic() - timer_started) * 1000)
        result["timer"]["remainingMs"] = max(0, state["timer"]["remainingMs"] - elapsed)
        if result["timer"]["remainingMs"] == 0:
            state["timer"]["running"] = False
    return result


class PreviewHandler(BaseHTTPRequestHandler):
    def log_message(self, fmt, *args):
        print(f"[preview] {self.address_string()} {fmt % args}")

    def send_json(self, data, status=200):
        payload = json.dumps(data).encode()
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(payload)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(payload)

    def read_json(self):
        size = int(self.headers.get("Content-Length", 0))
        return json.loads(self.rfile.read(size) or b"{}")

    def do_GET(self):
        if self.path in ("/", "/index.html", "/hotspot-detect.html"):
            payload = load_firmware_ui()
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(payload)))
            self.send_header("Cache-Control", "no-store")
            self.end_headers()
            self.wfile.write(payload)
        elif self.path == "/api/v1/status":
            self.send_json({"success": True, "data": current_status(), "error": None})
        elif self.path == "/api/v1/network/scan":
            networks = [
                {"ssid": "Home Wi-Fi", "rssi": -42, "secure": True},
                {"ssid": "PulseBar Demo", "rssi": -61, "secure": True},
                {"ssid": "Guest", "rssi": -74, "secure": False},
            ]
            self.send_json({"success": True, "data": networks, "error": None})
        elif self.path == "/api/v1/accounts":
            self.send_json({"success": True, "data": accounts, "error": None})
        elif self.path == "/api/v1/playlist":
            self.send_json({"success": True, "data": playlist, "error": None})
        else:
            self.send_json({"success": False, "data": None, "error": {"code": "NOT_FOUND", "message": "Route not found"}}, 404)

    def do_POST(self):
        global stopwatch_started, timer_started
        try:
            body = self.read_json()
            if self.path == "/api/v1/setup":
                state["name"] = body.get("deviceName") or "My PulseBar"
                state["ssid"] = body.get("ssid") or "Home Wi-Fi"
                state["timezone"] = body.get("timezone") or "UTC0"
                state["setupRequired"] = False
                self.send_json({"success": True, "data": {"message": "Desktop preview configured."}, "error": None})
                return
            if self.path == "/api/v1/accounts":
                platform, url = body.get("platform", ""), body.get("url", "")
                if platform not in accounts or not url.startswith(("http://", "https://")):
                    self.send_json({"success": False, "data": None, "error": {"code": "INVALID_ACCOUNT", "message": "Enter a complete channel link."}}, 400)
                    return
                accounts[platform] = {"url": url, "connected": True, "status": "Link saved — API authorization required for live metrics"}
                self.send_json({"success": True, "data": {"message": "Channel link saved"}, "error": None})
                return
            if self.path != "/api/v1/control":
                self.send_json({"success": False, "data": None, "error": {"code": "NOT_FOUND", "message": "Route not found"}}, 404)
                return
            action = body.get("action", "")
            if action == "brightness":
                state["brightness"] = max(1, min(255, int(body.get("value", 48))))
            elif action == "color":
                value = str(body.get("value", "#FF4D91"))
                if not re.fullmatch(r"#[0-9a-fA-F]{6}", value):
                    raise ValueError("Invalid display color")
                state["color"] = value.upper()
            elif action == "stopwatchStart" and not state["stopwatch"]["running"]:
                state["stopwatch"]["running"] = True
                stopwatch_started = time.monotonic()
            elif action == "stopwatchPause" and state["stopwatch"]["running"]:
                state["stopwatch"]["elapsedMs"] = current_status()["stopwatch"]["elapsedMs"]
                state["stopwatch"]["running"] = False
                stopwatch_started = None
            elif action == "stopwatchReset":
                state["stopwatch"] = {"running": False, "elapsedMs": 0}
                stopwatch_started = None
            elif action == "timerStart":
                state["timer"] = {"running": True, "remainingMs": max(1, int(body.get("seconds", 60))) * 1000}
                timer_started = time.monotonic()
            elif action == "timerPause" and state["timer"]["running"]:
                state["timer"]["remainingMs"] = current_status()["timer"]["remainingMs"]
                state["timer"]["running"] = False
                timer_started = None
            elif action == "timerResume" and state["timer"]["remainingMs"]:
                state["timer"]["running"] = True
                timer_started = time.monotonic()
            elif action == "timerReset":
                state["timer"] = {"running": False, "remainingMs": 0}
                timer_started = None
            elif action != "displayTest":
                self.send_json({"success": False, "data": None, "error": {"code": "UNKNOWN_ACTION", "message": "Unknown action"}}, 400)
                return
            self.send_json({"success": True, "data": {}, "error": None})
        except (ValueError, json.JSONDecodeError) as error:
            self.send_json({"success": False, "data": None, "error": {"code": "INVALID_REQUEST", "message": str(error)}}, 400)

    def do_PUT(self):
        try:
            body = self.read_json()
            if self.path != "/api/v1/playlist" or not isinstance(body.get("scenes"), list):
                self.send_json({"success": False, "data": None, "error": {"code": "INVALID_PLAYLIST", "message": "Invalid playlist"}}, 400)
                return
            known = {item["id"] for item in playlist["scenes"]}
            playlist["scenes"] = [{"id": item["id"], "enabled": bool(item.get("enabled"))} for item in body["scenes"] if item.get("id") in known]
            if not any(item["enabled"] for item in playlist["scenes"]):
                next(item for item in playlist["scenes"] if item["id"] == "time")["enabled"] = True
            self.send_json({"success": True, "data": playlist, "error": None})
        except (ValueError, json.JSONDecodeError) as error:
            self.send_json({"success": False, "data": None, "error": {"code": "INVALID_REQUEST", "message": str(error)}}, 400)


def main():
    parser = argparse.ArgumentParser(description="Run the PulseBar firmware UI on this PC")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8080)
    args = parser.parse_args()
    server = ThreadingHTTPServer((args.host, args.port), PreviewHandler)
    print(f"PulseBar desktop preview: http://{args.host}:{args.port}")
    print(f"Serving the live firmware UI from: {WEB_HEADER}")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()


if __name__ == "__main__":
    main()
