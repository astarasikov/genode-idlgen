---
Introduction
---
This repository will contain the RPC generator for Genode

---
Use case
---

Generate session and driver interfaces for a multi-resource
provider.

An example might be a GPIO controller sharing a register for
multiple pins. We want to limit the client access to only
a specific pin.

Therefore, the Session Root_component creates the Session_component
that calls the actual gpio driver. The Session_component executes
in the address space of our GPIO service and knows the pin number.

The client (a malicious touchscreen driver) does not know the pin,
and the interface between 'us' and the client does not involve the pin
