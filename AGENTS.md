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

## Development Workflow

All development is done in the **devel tree** (`~/Documents/ksf_FA_Shipping`). Do **not** edit files in the UAT bind point directly.

### Workflow Steps
1. **Develop** in this repo (feature branches preferred)
2. **Test**: run repo-appropriate tests
3. **Lint**: `php -l` on modified PHP files (no syntax errors)
4. **Commit** and **Push** branch to GitHub
5. **Merge** to `master` when ready
6. **Push** `master` to GitHub
7. **Deploy** to UAT by pulling in the Infrastructure bind point:

   ```
   cd ~/ksf_Infrastructure/fa_modules/ksf_FA_Shipping
   git stash -u
   git pull origin master
   git stash pop
   ```

### UAT Bind Point
| Path | Purpose |
|------|---------|
| `~/Documents/ksf_FA_Shipping` | Devel tree — all development, testing, commits |
| `~/ksf_Infrastructure/fa_modules/ksf_FA_Shipping` | UAT bind point — deployment target, integration testing (if mirrored) |

