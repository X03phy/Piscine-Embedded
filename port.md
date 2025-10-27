`DDRx`, `PORTx`, et `PINx` sont les trois piliers pour contrôler les broches d'entrée/sortie (GPIO - General Purpose Input/Output) sur un microcontrôleur AVR comme l'ATmega328P.

Voici l'explication :

-----

### Le concept de "Port"

D'abord, le **`x`** dans `DDRx`, `PORTx`, et `PINx` fait référence à un **Port**. Un port est simplement un groupe de broches (généralement 8) que le microcontrôleur gère ensemble.

Sur l'ATmega328P, vous avez principalement trois ports :

  * **Port B** (géré par `DDRB`, `PORTB`, `PINB`) - Correspond aux broches 0 à 7 du Port B (PB0 à PB7). Sur un Arduino Uno, cela inclut les broches numériques 8 à 13, ainsi que les broches du cristal.
  * **Port C** (géré par `DDRC`, `PORTC`, `PINC`) - Correspond aux broches PC0 à PC6. Sur un Uno, ce sont les broches d'entrée analogiques A0 à A5 (et A6/A7 sur certaines versions SMD).
  * **Port D** (géré par `DDRD`, `PORTD`, `PIND`) - Correspond aux broches PD0 à PD7. Sur un Uno, ce sont les broches numériques 0 à 7 (qui incluent la liaison série RX/TX).

Chacun de ces registres est un registre 8 bits, où **chaque bit (de 0 à 7) contrôle la broche correspondante du port**.

  * Le bit 0 contrôle la broche 0 du port (ex: `PB0`)
  * Le bit 1 contrôle la broche 1 du port (ex: `PB1`)
  * ...
  * Le bit 7 contrôle la broche 7 du port (ex: `PB7`)

-----

### DDRx (Data Direction Register) - Le "Directeur"

Son nom dit tout : il définit la **direction** des données pour chaque broche du port.

  * **Rôle :** Décider si une broche est une **ENTRÉE** (Input) ou une **SORTIE** (Output).
  * **Fonctionnement :**
      * Mettre un bit à **`0`** configure la broche correspondante en **ENTRÉE**.
      * Mettre un bit à **`1`** configure la broche correspondante en **SORTIE**.

-----

### PORTx (Port Data Register) - Le "Contrôleur"

C'est le registre le plus polyvalent. Son comportement **dépend de ce que vous avez défini dans `DDRx`**.

  * **Rôle :** Écrire une valeur (si en sortie) ou activer/désactiver la résistance de pull-up (si en entrée).

#### Cas 1 : La broche est configurée en SORTIE (DDRx bit = 1)

  * **Fonctionnement :** `PORTx` **définit l'état logique de la broche**.
      * Mettre un bit à **`0`** met la broche à l'état **BAS** (LOW, 0V).
      * Mettre un bit à **`1`** met la broche à l'état **HAUT** (HIGH, 5V).

**Analogie :** C'est l'interrupteur. `1` = Allumer la lumière, `0` = Éteindre la lumière.

#### Cas 2 : La broche est configurée en ENTRÉE (DDRx bit = 0)

  * **Fonctionnement :** `PORTx` **contrôle la résistance de "pull-up" interne**. C'est crucial pour éviter les états "flottants" (ni 0V, ni 5V).
      * Mettre un bit à **`0`** désactive le pull-up. La broche est en "haute impédance" (elle flotte).
      * Mettre un bit à **`1`** **active** le pull-up interne. La broche est "tirée" vers 5V par une résistance. Si rien n'est connecté, la lecture de la broche donnera 1 (HIGH). C'est idéal pour les boutons.

**Analogie :** C'est un "ressort de rappel" pour un bouton. `1` = le ressort est actif (le bouton est tiré vers le HAUT), `0` = le ressort est inactif (le bouton flotte).

-----

### PINx (Port Input Pins Register) - Le "Lecteur"

C'est le registre le plus simple des trois.

  * **Rôle :** **Lire** l'état logique *actuel* et *électrique* des broches du port.
  * **Fonctionnement :** C'est un registre en **lecture seule** (Read-Only).
      * Si vous lisez `PINx` et qu'un bit est à **`0`**, la broche correspondante est à l'état **BAS** (0V).
      * Si vous lisez `PINx` et qu'un bit est à **`1`**, la broche correspondante est à l'état **HAUT** (5V).

Peu importe que la broche soit une entrée ou une sortie. `PINx` vous donne toujours la "vérité" de ce qui se passe électriquement sur la broche.

**Analogie :** C'est le voltmètre. Il vous dit juste "Ici, je mesure 5V" ou "Ici, je mesure 0V".

-----

### Résumé

| Registre | Rôle principal | Si `DDRx` bit = 1 (Sortie) | Si `DDRx` bit = 0 (Entrée) |
| :--- | :--- | :--- | :--- |
| **`DDRx`** | Définit la **Direction** | `1` = Sortie | `0` = Entrée |
| **`PORTx`**| **Contrôle** / Écrit | `1` = Sortie HIGH<br>`0` = Sortie LOW | `1` = Activer Pull-up<br>`0` = Désactiver Pull-up (flottant) |
| **`PINx`** | **Lit** l'état | Lit l'état de la broche (1 ou 0) | Lit l'état de la broche (1 ou 0) |

-----


### Un mot sur les opérateurs `|=`, `&=`, `~`, `<<`

Vous verrez presque toujours ces opérateurs (opérateurs bitwise) :

  * `(1 << N)` : Crée un masque binaire avec un `1` à la position `N`. (ex: `(1 << 5)` donne `0b00100000`).
  * `REG |= (1 << N)` : **Met le bit N à 1** (Set). Utilise le `OR` logique. (Allume la LED, active le pull-up).
  * `REG &= ~(1 << N)` : **Met le bit N à 0** (Clear). Utilise le `AND` logique avec un masque inversé (`~`). (Éteint la LED, désactive le pull-up).
  * `REG & (1 << N)` : **Teste le bit N**. Le résultat est non-nul (vrai) si le bit est à 1, et nul (faux) si le bit est à 0. (Lit l'état du bouton).

