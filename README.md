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

In the lab deployment the daemon listens on `0.0.0.0:47017`. The OpenStack
security group does not expose that port to the public internet; it is reachable
from the student workstation over the private lab network.
