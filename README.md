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

The service reads an operator token into a fixed-size stack buffer. The lab
binary also contains a root shell helper, so the intended exploit is a direct
return-address overwrite to that helper.

In the lab deployment the daemon listens on `127.0.0.1:47017`. It is reachable
only from the web host itself after the first stage has provided a shell.
