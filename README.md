# Carbon Telemetryd

Local privileged service used in the FHNW exploitation lab.

Build:

```bash
make
```

Run locally:

```bash
sudo ./carbon_telemetryd
```

The service reads an operator token into a fixed-size stack buffer and exposes
an executable stack plus a `jmp_rsp` helper for shellcode-driven exploitation.
