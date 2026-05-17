# Embedded Firmware CI/CD Demo — Azure DevOps

> **No physical device required** — all stages run on Azure DevOps using
> host builds, Unity unit tests, and QEMU emulation.

---

## Project Structure

```
firmware-cicd-demo/
├── src/
│   ├── main.c                  # Firmware entry point
│   └── drivers/
│       ├── gpio.c / gpio.h     # GPIO driver
│       └── uart.c / uart.h     # UART driver
├── tests/
│   ├── test_gpio.c             # Unity GPIO tests
│   └── test_uart.c             # Unity UART tests
├── lib/
│   └── Unity/                  # Unity test framework
│       ├── unity.c
│       ├── unity.h
│       └── unity_internals.h
├── scripts/
│   ├── smoke_test.py           # Post-flash serial validator
│   └── sign_firmware.sh        # SHA256 + GPG signing
├── .azure/
│   └── azure-pipelines.yml     # Full CI/CD pipeline (8 stages)
├── CMakeLists.txt              # Dual-mode build (HOST + ARM)
├── linker.ld                   # STM32F4 linker script
└── README.md
```

---

## Pipeline Stages (Azure DevOps)

| # | Stage | What Runs | Device Needed? |
|---|-------|-----------|----------------|
| 1 | 🔨 Build | ARM cross-compile + host build | ❌ No |
| 2 | 🔍 Static Analysis | Cppcheck on all source | ❌ No |
| 3 | 🧪 Unit Tests | Unity tests (GPIO, UART) on CI agent | ❌ No |
| 4 | 📊 Coverage | lcov HTML coverage report | ❌ No |
| 5 | 🖥️ QEMU Emulation | Boot firmware, check BOOT_OK | ❌ No |
| 6 | 📦 Sign & Package | SHA256 checksums + manifest | ❌ No |
| 7 | 🚀 Deploy Dev | Simulated flash + smoke test | ❌ No |
| 8 | 🏭 Deploy Prod | Manual approval → OTA push (simulated) | ❌ No |

---

## Quick Start

### 1. Run unit tests locally

```bash
# Prerequisites: cmake, gcc, lcov
cmake -B build -DHOST=ON -DCOVERAGE=ON
cmake --build build --parallel
./build/test_gpio
./build/test_uart
```

### 2. Generate coverage report locally

```bash
lcov --capture --directory build --output-file coverage.info
genhtml coverage.info --output-directory coverage-html
open coverage-html/index.html   # macOS
xdg-open coverage-html/index.html  # Linux
```

### 3. Run the smoke test in simulate mode

```bash
python3 scripts/smoke_test.py --simulate --expect BOOT_OK
```

### 4. (Optional) ARM cross-compile

```bash
# Requires: arm-none-eabi-gcc
sudo apt-get install gcc-arm-none-eabi
cmake -B build-arm -DHOST=OFF
cmake --build build-arm
ls build-arm/firmware.*   # .elf, .bin, .hex
```

---

## Azure DevOps Setup

### Step 1 — Import repo
1. Go to Azure DevOps → **Repos** → Import Repository
2. Paste your GitHub URL or upload these files

### Step 2 — Create pipeline
1. Go to **Pipelines** → **New Pipeline**
2. Select **Azure Repos Git** → select your repo
3. Choose **Existing Azure Pipelines YAML file**
4. Path: `.azure/azure-pipelines.yml`

### Step 3 — Create environments
1. Go to **Pipelines** → **Environments**
2. Create `dev` (auto-approve)
3. Create `production` → Add **Approvals & Checks** → Approvals → add yourself

### Step 4 — Set pipeline variables (optional)
| Variable | Value | Secret? |
|----------|-------|---------|
| `FIRMWARE_VER` | `1.0.0` | No |
| `GPG_PRIVATE_KEY` | your key | ✅ Yes |
| `GPG_PASSPHRASE` | passphrase | ✅ Yes |

### Step 5 — Run
Push any commit to `main` → pipeline triggers automatically.

---

## What to Show in a Demo / Interview

1. **Pipeline overview** → 8 green stages in the Azure DevOps UI
2. **Unit test results** → Tests tab showing 13 tests, all passing
3. **Coverage report** → HTML report tab showing % lines covered
4. **QEMU emulation log** → `BOOT_OK` and `SELF_TEST_PASS` in logs
5. **Artifacts** → Download `firmware.bin`, `firmware.hex`, `MANIFEST.txt`
6. **Production gate** → Show the manual approval waiting state

---

## Extending with Real Hardware

When you get access to a physical STM32 board, replace the simulated
deploy stage with:

```yaml
- task: Bash@3
  displayName: "Flash via OpenOCD"
  inputs:
    targetType: inline
    script: |
      openocd -f interface/stlink.cfg \
              -f target/stm32f4x.cfg \
              -c "program firmware.bin verify reset exit 0x08000000"
```

And the smoke test:

```bash
python3 scripts/smoke_test.py \
  --port /dev/ttyUSB0 \
  --baud 115200 \
  --expect BOOT_OK
```

---

## License

MIT — free to use, modify, and showcase.
