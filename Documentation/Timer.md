# 🕒 Les Timers sur microcontrôleurs AVR

## 1. Introduction

Les **timers** (ou **compteurs**) sont des **périphériques matériels intégrés** au microcontrôleur.  
Ils permettent de **mesurer le temps**, **générer des signaux périodiques**, ou **produire des interruptions à intervalles réguliers**, sans bloquer le CPU.

Les timers sont essentiels pour :
- faire clignoter une LED à fréquence fixe,
- mesurer une durée précise,
- générer un signal PWM,
- déclencher une action automatique sans attendre dans une boucle.

---

## 2. Types de timers

Sur un **ATmega328P** (comme celui d’un Arduino Uno), on trouve :

| Timer | Taille | Usage typique |
|--------|---------|----------------|
| Timer0 | 8 bits  | PWM, millis() |
| Timer1 | 16 bits | tâches précises, signaux lents |
| Timer2 | 8 bits  | PWM ou génération de sons |

> Le **Timer1** (16 bits) est le plus précis pour des périodes longues, comme un clignotement de 1 Hz.

---

## 3. Structure d’un timer

Chaque timer possède :

- **Un compteur** : `TCNTx` → il s’incrémente à chaque « tick » d’horloge.  
- **Une valeur de comparaison** : `OCRxA`, `OCRxB` → quand le compteur atteint cette valeur, une action peut être déclenchée.  
- **Des registres de contrôle** : `TCCRxA`, `TCCRxB` → permettent de choisir le mode, le prescaler, etc.  
- **Des bits d’interruption** : `TIMSKx`, `TIFRx`.

---

## 4. Source d’horloge et prescaler

Le compteur ne s’incrémente pas à chaque cycle CPU, mais selon un **prescaler**.  
Celui-ci divise la fréquence du microcontrôleur (`F_CPU`) :

| Bits `CSn2:CSn0` | Prescaler | Fréquence timer si F_CPU = 8 MHz |
|-------------------|-----------|----------------------------------|
| 001 | 1 | 8 MHz |
| 010 | 8 | 1 MHz |
| 011 | 64 | 125 kHz |
| 100 | 256 | 31.25 kHz |
| 101 | 1024 | 7.8125 kHz |

> Exemple : avec un prescaler de 1024, le timer incrémente son compteur 7812 fois par seconde.

---

## 5. Modes de fonctionnement

| Mode | Bits WGM | Description |
|------|-----------|-------------|
| **Normal** | `0000` | Le compteur compte jusqu’à 65535, puis repart à 0. |
| **CTC (Clear Timer on Compare)** | `0100` | Le compteur repart à 0 dès qu’il atteint `OCR1A`. |
| **PWM** | autres | Utilisé pour générer des signaux modulés. |

Le mode **CTC** est très utile pour générer des événements périodiques précis (comme un clignotement).

---

## 6. Actions sur les sorties (COM bits)

Quand une comparaison se produit (`TCNTx = OCRxA`), on peut demander au matériel d’agir sur une **sortie spéciale** :  
`OC1A`, `OC1B`, etc. (elles correspondent à des broches physiques du microcontrôleur).

Les bits `COM1A1:COM1A0` définissent cette action :

| COM1A1 | COM1A0 | Action sur OC1A |
|--------|---------|-----------------|
| 0 | 0 | Aucune |
| 0 | 1 | **Toggle** (inversion automatique) |
| 1 | 0 | Clear (passe à 0) |
| 1 | 1 | Set (passe à 1) |

> Le mode **Toggle** permet de faire clignoter une LED automatiquement, sans toucher à `PORTB`.
