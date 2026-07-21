#!/usr/bin/env python3
"""Create a production PulseBar identity and one-time customer claim code."""

import argparse
import json
import urllib.request


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--backend", required=True, help="PulseBar Connect HTTPS URL")
    parser.add_argument("--admin-token", required=True, help="ADMIN_TOKEN configured on the backend")
    parser.add_argument("--device-id", required=True, help="Unique serial, for example PB-000001")
    args = parser.parse_args()
    payload = json.dumps({"deviceId": args.device_id}).encode()
    request = urllib.request.Request(
        args.backend.rstrip("/") + "/v1/admin/devices",
        data=payload,
        headers={"Authorization": "Bearer " + args.admin_token, "Content-Type": "application/json"},
        method="POST",
    )
    with urllib.request.urlopen(request) as response:
        device = json.load(response)
    print("\nPulseBar manufacturing record — store securely and do not commit\n")
    print("Device ID:    " + device["deviceId"])
    print("Device secret:" + device["deviceSecret"])
    print("Claim code:   " + device["claimCode"])
    print("\nEnter the ID and secret in the device installer settings.")
    print("Print only the Device ID and Claim code on the customer card/QR label.")


if __name__ == "__main__":
    main()
