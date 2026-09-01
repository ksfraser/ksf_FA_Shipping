<!-- Repo-specific appendix to the shared AGENTS.md. Generic conventions live in AGENTS_ARCH.md (hardlinked). -->

```markdown
# AGENTS.local.md — ksf_FA_Shipping
## Overview
**FA Module** for Shipping Rate Calculator — extracted from WooCommerce Shipping Method API; supports real-time rate calculation, zone-based rules, and carrier integration.
## Repository Structure
```
ksf_FA_Shipping/
├── sql/
│   ├── fa_shipping_zones.sql
│   ├── fa_shipping_methods.sql
│   ├── fa_shipping_rates.sql
│   └── fa_shipping_carrier_logs.sql
├── includes/
│   ├── shipping_zones_db.inc
│   ├── shipping_rates_db.inc
│   └── ...
├── src/
│   ├── Contracts/
│   │   └── ShippingCalculatorInterface
│   ├── Services/
│   │   ├── RateCalculator
│   │   └── ZoneMatcher
│   └── Carriers/
│       └── Carrier strategy implementations
├── pages/
├── hooks.php
├── composer.json
└── ProjectDocs/
    ├── Requirements.md
    ├── RTM.md
    ├── BABOK.md
    └── UML.md
```
## Dependencies
- **ksf_FA_Shipping_Core** (business logic - extracted from WooCommerce)
- **ksf_FA_CRM** (customer addresses for shipping)
- **FrontAccounting 2.4+**
```
