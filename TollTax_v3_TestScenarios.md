# Toll Tax Management System v3.0 — Simulated Test Scenario Outputs

---

## Scenario 1 — ALPR Tamper Attempt

```
  ╔══════════════════════════════════════════╗
  ║   TOLL TAX MANAGEMENT SYSTEM  v3.0       ║
  ║   STOP-FREE AI AUTOMATION EDITION        ║
  ║   SRM University-AP  |  OOP C++ Project  ║
  ╚══════════════════════════════════════════╝
  Team: Lokesh | Mathews | Jishnu | Rohit | Greysi


  ===== MAIN MENU =====
  1.  Car
  2.  Bus
  3.  Truck
  4.  Bike
  5.  Ambulance
  ─────────────────────
  6.  Show Session Total
  7.  Show Exempt Vehicle Log
  8.  Save Session Data to File
  9.  FASTag Wallet Management
  10. Database Summary
  0.  Exit & Save
  ─────────────────────
  Choice: 1
  ----------------------------------------
  License Plate (e.g. AP09AB1234): AP09AB1234
  Distance (km): 10
  Vehicle weight in tons (sensor reading): 1.5

  ########################################
  [AI PIPELINE] Starting verification for : AP09AB1234
  [AI PIPELINE] Timestamp                 : 2025-11-14 11:32:07
  ########################################

  STAGE 1  ► Dual-Camera ALPR Verification
  ----------------------------------------

  [CAM-1 FRONT]  Read : AP09AB1234
  [CAM-2 REAR ]  Read : AP09AB1235
  [ALPR] *** PLATE MISMATCH DETECTED ***
  [ALPR] Front: AP09AB1234  vs  Rear: different reading.
  [ALPR] Possible plate swap / tampering. BARRIER LOCKED.

  !! [SECURITY ALERT — OFFICER NOTIFIED] !!
  Reason  : License Plate Mismatch (Tampering)
  Plate   : AP09AB1234
  Action  : BARRIER LOCKED — Officer dispatched
  Time    : 2025-11-14 11:32:07

  [SYSTEM] Entry DENIED by AI pipeline. Incident logged.

  ===== MAIN MENU =====
  ...
  Choice: 0
  ----------------------------------------
  Session total saved to 'daily_collection.txt'.

  Session data saved. Thank you! Goodbye.
```

---

## Scenario 2 — Stolen Vehicle Flag

```
  ╔══════════════════════════════════════════╗
  ║   TOLL TAX MANAGEMENT SYSTEM  v3.0       ║
  ║   STOP-FREE AI AUTOMATION EDITION        ║
  ║   SRM University-AP  |  OOP C++ Project  ║
  ╚══════════════════════════════════════════╝
  Team: Lokesh | Mathews | Jishnu | Rohit | Greysi


  ===== MAIN MENU =====
  1.  Car
  2.  Bus
  3.  Truck
  4.  Bike
  5.  Ambulance
  ─────────────────────
  6.  Show Session Total
  7.  Show Exempt Vehicle Log
  8.  Save Session Data to File
  9.  FASTag Wallet Management
  10. Database Summary
  0.  Exit & Save
  ─────────────────────
  Choice: 1
  ----------------------------------------
  License Plate (e.g. AP09AB1234): TN22GH9999
  Distance (km): 10
  Vehicle weight in tons (sensor reading): 1.5

  ########################################
  [AI PIPELINE] Starting verification for : TN22GH9999
  [AI PIPELINE] Timestamp                 : 2025-11-14 11:35:22
  ########################################

  STAGE 1  ► Dual-Camera ALPR Verification
  ----------------------------------------

  [CAM-1 FRONT]  Read : TN22GH9999
  [CAM-2 REAR ]  Read : TN22GH9999
  [ALPR] Both cameras AGREE. Verified plate: TN22GH9999  ✓

  STAGE 2  ► Central Government Database Check
  ----------------------------------------
  [DATABASE] Owner  : Unknown
  [DATABASE] Plate  : TN22GH9999
  [DATABASE] Status : *** STOLEN VEHICLE FLAGGED ***

  !! [SECURITY ALERT — OFFICER NOTIFIED] !!
  Reason  : Stolen Vehicle Identified
  Plate   : TN22GH9999
  Action  : BARRIER LOCKED — Officer dispatched
  Time    : 2025-11-14 11:35:22

  [SYSTEM] Entry DENIED by AI pipeline. Incident logged.

  ===== MAIN MENU =====
  ...
  Choice: 0
  ----------------------------------------
  Session total saved to 'daily_collection.txt'.

  Session data saved. Thank you! Goodbye.
```

---

## Scenario 3 — Unpaid Fines Notice

```
  ╔══════════════════════════════════════════╗
  ║   TOLL TAX MANAGEMENT SYSTEM  v3.0       ║
  ║   STOP-FREE AI AUTOMATION EDITION        ║
  ║   SRM University-AP  |  OOP C++ Project  ║
  ╚══════════════════════════════════════════╝
  Team: Lokesh | Mathews | Jishnu | Rohit | Greysi


  ===== MAIN MENU =====
  1.  Car
  2.  Bus
  3.  Truck
  4.  Bike
  5.  Ambulance
  ─────────────────────
  6.  Show Session Total
  7.  Show Exempt Vehicle Log
  8.  Save Session Data to File
  9.  FASTag Wallet Management
  10. Database Summary
  0.  Exit & Save
  ─────────────────────
  Choice: 1
  ----------------------------------------
  License Plate (e.g. AP09AB1234): KA05MN3344
  Distance (km): 10
  Vehicle weight in tons (sensor reading): 1.5

  ########################################
  [AI PIPELINE] Starting verification for : KA05MN3344
  [AI PIPELINE] Timestamp                 : 2025-11-14 11:40:15
  ########################################

  STAGE 1  ► Dual-Camera ALPR Verification
  ----------------------------------------

  [CAM-1 FRONT]  Read : KA05MN3344
  [CAM-2 REAR ]  Read : KA05MN3344
  [ALPR] Both cameras AGREE. Verified plate: KA05MN3344  ✓

  STAGE 2  ► Central Government Database Check
  ----------------------------------------
  [DATABASE] Owner  : Rajan S.
  [DATABASE] Plate  : KA05MN3344
  [DATABASE] Status : UNPAID FINES — Rs.850.00

  [FINE NOTICE]
  To      : Rajan S.
  Plate   : KA05MN3344
  Fine    : Rs.850.00 outstanding
  Pay at  : https://echallan.parivahan.gov.in
  Note    : Entry allowed. Fine must be cleared within 30 days.
  [DATABASE] Action : Entry allowed. Fine notice sent.

  STAGE 3  ► AI Weight Fraud Detection
  ----------------------------------------
  [AI-WEIGHT]   Car @ 1.50 tons — within legal range. PASS.

  ########################################
  [AI PIPELINE] ALL STAGES PASSED. Barrier opening...
  ########################################
  Pass? (0=None  1=Daily  2=Monthly): 0
  Return Journey? (0=No  1=Yes): 0
  [AUTO-BILLING] No FASTag linked to this plate.
  Use FASTag manually? (0=Cash  1=Yes): 0

  Vehicle     : Car
  Plate       : KA05MN3344
  Distance    : 10 km
  Base Toll   : Rs.20.00
  Final Toll  : Rs.20.00
  [Cash] Toll Rs.20.00 collected at booth.

  ┌──────────────────────────────────────────┐
  │         [SMS NOTIFICATION SENT]          │
  ├──────────────────────────────────────────┤
  │  To       : Rajan S.
  │  Vehicle  : KA05MN3344
  │  Type     : Car
  │  Toll     : Rs.20.00
  │  Payment  : Cash collected at booth
  │  Time     : 2025-11-14 11:40:15
  │  Status   : PAID  ✓  BARRIER OPENED
  └──────────────────────────────────────────┘

  ===== MAIN MENU =====
  ...
  Choice: 0
  ----------------------------------------
  Session total saved to 'daily_collection.txt'.

  Session data saved. Thank you! Goodbye.
```

---

## Scenario 4 — AI Weight Fraud (Jishnu's Plate, 15.0 t)

```
  ╔══════════════════════════════════════════╗
  ║   TOLL TAX MANAGEMENT SYSTEM  v3.0       ║
  ║   STOP-FREE AI AUTOMATION EDITION        ║
  ║   SRM University-AP  |  OOP C++ Project  ║
  ╚══════════════════════════════════════════╝
  Team: Lokesh | Mathews | Jishnu | Rohit | Greysi


  ===== MAIN MENU =====
  1.  Car
  2.  Bus
  3.  Truck
  4.  Bike
  5.  Ambulance
  ─────────────────────
  6.  Show Session Total
  7.  Show Exempt Vehicle Log
  8.  Save Session Data to File
  9.  FASTag Wallet Management
  10. Database Summary
  0.  Exit & Save
  ─────────────────────
  Choice: 1
  ----------------------------------------
  License Plate (e.g. AP09AB1234): AP09EF9012
  Distance (km): 10
  Vehicle weight in tons (sensor reading): 15.0

  ########################################
  [AI PIPELINE] Starting verification for : AP09EF9012
  [AI PIPELINE] Timestamp                 : 2025-11-14 11:45:03
  ########################################

  STAGE 1  ► Dual-Camera ALPR Verification
  ----------------------------------------

  [CAM-1 FRONT]  Read : AP09EF9012
  [CAM-2 REAR ]  Read : AP09EF9012
  [ALPR] Both cameras AGREE. Verified plate: AP09EF9012  ✓

  STAGE 2  ► Central Government Database Check
  ----------------------------------------
  [DATABASE] Owner  : Jishnu
  [DATABASE] Plate  : AP09EF9012
  [DATABASE] Status : CLEAR  ✓

  STAGE 3  ► AI Weight Fraud Detection
  ----------------------------------------

  ╔══════════════════════════════════════════╗
  ║       *** AI FRAUD ALERT RAISED ***      ║
  ╚══════════════════════════════════════════╝
  Plate         : AP09EF9012
  Declared type : Car
  Sensor weight : 15.00 tons
  Legal range   : 0.80 – 3.50 tons
  AI Decision   : MISMATCH — BARRIER LOCKED
  [LOG] Incident written to fraud_log.txt

  !! [SECURITY ALERT — OFFICER NOTIFIED] !!
  Reason  : Weight Fraud: Car declared but 15.000000t detected
  Plate   : AP09EF9012
  Action  : BARRIER LOCKED — Officer dispatched
  Time    : 2025-11-14 11:45:03

  [SYSTEM] Entry DENIED by AI pipeline. Incident logged.

  ===== MAIN MENU =====
  ...
  Choice: 0
  ----------------------------------------
  Session total saved to 'daily_collection.txt'.

  Session data saved. Thank you! Goodbye.
```

---

## Scenario 5 — Perfect Stop-Free Auto-Billing (Lokesh's Plate)

```
  ╔══════════════════════════════════════════╗
  ║   TOLL TAX MANAGEMENT SYSTEM  v3.0       ║
  ║   STOP-FREE AI AUTOMATION EDITION        ║
  ║   SRM University-AP  |  OOP C++ Project  ║
  ╚══════════════════════════════════════════╝
  Team: Lokesh | Mathews | Jishnu | Rohit | Greysi


  ===== MAIN MENU =====
  1.  Car
  2.  Bus
  3.  Truck
  4.  Bike
  5.  Ambulance
  ─────────────────────
  6.  Show Session Total
  7.  Show Exempt Vehicle Log
  8.  Save Session Data to File
  9.  FASTag Wallet Management
  10. Database Summary
  0.  Exit & Save
  ─────────────────────
  Choice: 1
  ----------------------------------------
  License Plate (e.g. AP09AB1234): AP09AB1234
  Distance (km): 10
  Vehicle weight in tons (sensor reading): 1.5

  ########################################
  [AI PIPELINE] Starting verification for : AP09AB1234
  [AI PIPELINE] Timestamp                 : 2025-11-14 11:50:44
  ########################################

  STAGE 1  ► Dual-Camera ALPR Verification
  ----------------------------------------

  [CAM-1 FRONT]  Read : AP09AB1234
  [CAM-2 REAR ]  Read : AP09AB1234
  [ALPR] Both cameras AGREE. Verified plate: AP09AB1234  ✓

  STAGE 2  ► Central Government Database Check
  ----------------------------------------
  [DATABASE] Owner  : Lokesh
  [DATABASE] Plate  : AP09AB1234
  [DATABASE] Status : CLEAR  ✓

  STAGE 3  ► AI Weight Fraud Detection
  ----------------------------------------
  [AI-WEIGHT]   Car @ 1.50 tons — within legal range. PASS.

  ########################################
  [AI PIPELINE] ALL STAGES PASSED. Barrier opening...
  ########################################
  Pass? (0=None  1=Daily  2=Monthly): 0
  Return Journey? (0=No  1=Yes): 0
  [AUTO-BILLING] FASTag linked for Lokesh — will auto-deduct.

  Vehicle     : Car
  Plate       : AP09AB1234
  Distance    : 10 km
  Base Toll   : Rs.20.00
  Final Toll  : Rs.20.00
  [AUTO-BILL] FASTag deduction initiated...
  [FASTag OK] Rs.20.00 deducted | Balance: Rs.480.00

  ┌──────────────────────────────────────────┐
  │         [SMS NOTIFICATION SENT]          │
  ├──────────────────────────────────────────┤
  │  To       : Lokesh
  │  Vehicle  : AP09AB1234
  │  Type     : Car
  │  Toll     : Rs.20.00
  │  FASTag   : Rs.480.00 remaining
  │  Time     : 2025-11-14 11:50:44
  │  Status   : PAID  ✓  BARRIER OPENED
  └──────────────────────────────────────────┘

  ╔══════════════════════════════════════════╗
  ║      [PUSH NOTIFICATION SENT]            ║
  ╠══════════════════════════════════════════╣
  ║  Hi Lokesh!
  ║  Toll of Rs.20.00 deducted for AP09AB1234
  ║  Toll plaza cleared successfully.
  ║  Drive safe! — FASTag India
  ╚══════════════════════════════════════════╝

  ===== MAIN MENU =====
  ...
  Choice: 0
  ----------------------------------------
  Session total saved to 'daily_collection.txt'.

  Session data saved. Thank you! Goodbye.
```

---

## Scenario 6 — Ambulance Analytics & File Output

```
  ╔══════════════════════════════════════════╗
  ║   TOLL TAX MANAGEMENT SYSTEM  v3.0       ║
  ║   STOP-FREE AI AUTOMATION EDITION        ║
  ║   SRM University-AP  |  OOP C++ Project  ║
  ╚══════════════════════════════════════════╝
  Team: Lokesh | Mathews | Jishnu | Rohit | Greysi


  ===== MAIN MENU =====
  1.  Car
  2.  Bus
  3.  Truck
  4.  Bike
  5.  Ambulance
  ─────────────────────
  6.  Show Session Total
  7.  Show Exempt Vehicle Log
  8.  Save Session Data to File
  9.  FASTag Wallet Management
  10. Database Summary
  0.  Exit & Save
  ─────────────────────
  Choice: 5
  ----------------------------------------
  License Plate (e.g. AP09AB1234): TS09EM0001
  Distance (km): 15
  [SENSOR] Ambulance weight auto-read: 4.0 tons

  ########################################
  [AI PIPELINE] Starting verification for : TS09EM0001
  [AI PIPELINE] Timestamp                 : 2025-11-14 11:55:30
  ########################################

  STAGE 1  ► Dual-Camera ALPR Verification
  ----------------------------------------

  [CAM-1 FRONT]  Read : TS09EM0001
  [CAM-2 REAR ]  Read : TS09EM0001
  [ALPR] Both cameras AGREE. Verified plate: TS09EM0001  ✓

  STAGE 2  ► Central Government Database Check
  ----------------------------------------
  [DATABASE] Owner  : Guest (Unregistered)
  [DATABASE] Plate  : TS09EM0001
  [DATABASE] Status : CLEAR  ✓

  STAGE 3  ► AI Weight Fraud Detection
  ----------------------------------------
  [AI-WEIGHT]   Ambulance @ 4.00 tons — within legal range. PASS.

  ########################################
  [AI PIPELINE] ALL STAGES PASSED. Barrier opening...
  ########################################

  Vehicle     : Ambulance (Emergency Service)
  Plate       : TS09EM0001
  Toll        : Rs.0.00 — Fully Exempt
  [Analytics] Passage logged to exempt log.

  ┌──────────────────────────────────────────┐
  │         [SMS NOTIFICATION SENT]          │
  ├──────────────────────────────────────────┤
  │  To       : Emergency Services
  │  Vehicle  : TS09EM0001
  │  Type     : Ambulance
  │  Toll     : Rs.0.00
  │  Payment  : Cash collected at booth
  │  Time     : 2025-11-14 11:55:30
  │  Status   : PAID  ✓  BARRIER OPENED
  └──────────────────────────────────────────┘

  ===== MAIN MENU =====
  1.  Car
  2.  Bus
  3.  Truck
  4.  Bike
  5.  Ambulance
  ─────────────────────
  6.  Show Session Total
  7.  Show Exempt Vehicle Log
  8.  Save Session Data to File
  9.  FASTag Wallet Management
  10. Database Summary
  0.  Exit & Save
  ─────────────────────
  Choice: 7
  ----------------------------------------

===== Exempt Vehicle Analytics Log =====
  1. [2025-11-14 11:55:30]  Ambulance | Plate: TS09EM0001 | Dist: 15 km | Toll: Rs.0.00 (Emergency Exempt)

  ===== MAIN MENU =====
  ...
  Choice: 0
  ----------------------------------------
  Session total saved to 'daily_collection.txt'.

  Session data saved. Thank you! Goodbye.
```
