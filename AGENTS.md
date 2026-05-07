# AGENTS.md - ksf_FA_Shipping#

## Architecture Overview#

This repository implements **Shipping Rate Calculator** extracted from WooCommerce Shipping Method API - supports real-time rate calculation, zone-based rules, and carrier integration.

### Core Principles#
- **SOLID**: Single Responsibility, Open/Closed, Liskov Substitution, Interface Segregation, Dependency Inversion#
- **DRY**: Don't Repeat Yourself - extract reusable logic#
- **TDD**: Test-Driven Development - write tests first#
- **DI**: Dependency Injection - inject dependencies, don't hardcode#
- **SRP**: Single Responsibility Principle - each class has one reason to change#

## Repository Structure#

```
ksf_FA_Shipping/
├── sql/                    # Database schemas (FA TB_PREF tables)#
│   ├── fa_shipping_zones.sql#
│   ├── fa_shipping_methods.sql#
│   ├── fa_shipping_rates.sql#
│   └── fa_shipping_carrier_logs.sql#
├── includes/              # FA-specific DB classes#
│   ├── shipping_zones_db.inc#
│   ├── shipping_rates_db.inc#
│   └── ...#
├── src/                    # Business logic (namespace: Ksf\FA\Shipping\)#
│   ├── Contracts/        # ShippingCalculatorInterface#
│   ├── Services/         # RateCalculator, ZoneMatcher#
│   └── Carriers/        # Carrier strategy implementations#
├── pages/                 # UI pages (FA admin)#
├── hooks.php#
├── composer.json#
└── ProjectDocs/#
    ├── Requirements.md#
    ├── RTM.md#
    ├── BABOK.md#
    └── UML.md#
```

## Dependencies#

- **ksf_FA_Shipping_Core** (business logic - extracted from WooCommerce)#
- **ksf_FA_CRM** (customer addresses for shipping)#
- **FrontAccounting 2.4+**#
