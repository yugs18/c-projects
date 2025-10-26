# Projects

## *cli-calculator*
    It supports addition, subtraction, multiplication and division operations with BODMAS rule.
    Usage: cli-calculator <number1> <operator> <number2> <operator> <number3> ...


## 🧅 Toralize — Tunnel Any Linux App Through Tor (SOCKS4)

**Toralize** is a lightweight LD_PRELOAD-based tool that intercepts system network calls and redirects them through the **Tor network** using the **SOCKS4 protocol**.

It allows you to anonymize the network traffic of any Linux command-line application (like `curl`, `wget`, etc.) — **without modifying their source code**.

---

### ⚙️ How It Works

`toralize` hooks the `connect()` system call using `LD_PRELOAD`.  
When an application tries to connect to an IP/port, `toralize`:

1. Creates a new socket.
2. Connects it to the local Tor SOCKS proxy (default `127.0.0.1:9050`).
3. Sends a SOCKS4 request containing the target IP and port.
4. Forwards the application’s traffic through Tor.


### Example Ouput
```bash
$ toralize curl http://google.com

Connected to proxy successfully.
<HTML><HEAD><meta http-equiv="content-type" content="text/html;charset=utf-8">
<TITLE>301 Moved</TITLE></HEAD><BODY>
<H1>301 Moved</H1>
The document has moved <A HREF="http://www.google.com/">here</A>.
</BODY></HTML>