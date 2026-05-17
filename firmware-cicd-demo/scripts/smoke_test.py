#!/usr/bin/env python3
"""
smoke_test.py — Post-flash smoke test over serial port (or simulated output)

Usage (real device):
    python3 smoke_test.py --port /dev/ttyUSB0 --baud 115200 --expect BOOT_OK

Usage (simulated / CI without device):
    python3 smoke_test.py --simulate --expect BOOT_OK
"""

import argparse
import sys
import time


def run_real_serial(port: str, baud: int, expect: str, timeout: int) -> bool:
    try:
        import serial
    except ImportError:
        print("[ERROR] pyserial not installed. Run: pip install pyserial")
        return False

    try:
        ser = serial.Serial(port, baud, timeout=timeout)
        time.sleep(2)  # Wait for board to boot
        output = ser.read(1024).decode(errors="replace")
        ser.close()
        print(f"[OUTPUT]\n{output}")
        if expect in output:
            print(f"[PASS] '{expect}' found in serial output")
            return True
        else:
            print(f"[FAIL] '{expect}' NOT found in serial output")
            return False
    except Exception as e:
        print(f"[ERROR] Serial connection failed: {e}")
        return False


def run_simulated(expect: str) -> bool:
    """Simulate firmware output for CI environments without hardware."""
    simulated_output = (
        "==================================\r\n"
        "  Firmware v1.0.0\r\n"
        "  Azure DevOps CI/CD Demo\r\n"
        "==================================\r\n"
        "BOOT_OK\r\n"
        "LED ON  [1/5]\r\n"
        "LED OFF [1/5]\r\n"
        "LED ON  [2/5]\r\n"
        "LED OFF [2/5]\r\n"
        "SELF_TEST_PASS\r\n"
        "Entering main loop...\r\n"
    )
    print("[SIMULATED OUTPUT]")
    print(simulated_output)

    if expect in simulated_output:
        print(f"[PASS] '{expect}' found in simulated output")
        return True
    else:
        print(f"[FAIL] '{expect}' NOT found in simulated output")
        return False


def main():
    parser = argparse.ArgumentParser(description="Firmware smoke test")
    parser.add_argument("--port",     default="/dev/ttyUSB0", help="Serial port")
    parser.add_argument("--baud",     type=int, default=115200, help="Baud rate")
    parser.add_argument("--expect",   default="BOOT_OK",       help="Expected string")
    parser.add_argument("--timeout",  type=int, default=10,    help="Timeout (s)")
    parser.add_argument("--simulate", action="store_true",     help="Simulate (no device)")
    args = parser.parse_args()

    print(f"Smoke test: expecting '{args.expect}'")
    print(f"Mode: {'SIMULATED' if args.simulate else f'SERIAL {args.port} @ {args.baud}'}")
    print("-" * 40)

    if args.simulate:
        ok = run_simulated(args.expect)
    else:
        ok = run_real_serial(args.port, args.baud, args.expect, args.timeout)

    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
