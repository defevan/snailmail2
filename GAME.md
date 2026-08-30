# Snail Mail

Game Boy Color platformer. You are a snail. You deliver mail.

**Inspo:** Kirby’s Dream Land, Wario Land 2 (no death), Wario Land: Super Mario Land 3.

**Length:** ~45–60 min first clear. ~90 min with all stamps.

**Engine:** GB Studio 4.x platformer. Notes below are constraints, not a build plan.

---

## Premise

You are the valley’s only mail snail. Letters go missing — a Magpie keeps stealing the shiny ones. Find each letter and deliver it.

**Win:** Deliver all four letters and beat the Magpie at Parcel Peak.

**Win well:** Also collect the optional stamps. Stamp count changes the ending.

**Feel:** Soft and sturdy. Enemies are toys. Delivery is the point. Stamps are optional greed.

---

## Tone

- Soft slapstick. Never cruel.
- Clear silhouettes. Thick outlines. Room to breathe on screen.
- No death. Hits and pits only delay or divert you. Combat is optional when layout allows.

---

## Structure

| Chunk | Count | Role | ~Time |
|-------|------:|------|------:|
| Title / intro | 1 | Learn controls in play | 2 min |
| Stamp Square (hub) | 1 | Pick next route | revisit |
| Delivery routes | **4** | One letter each; one new twist each | 8–10 min each |
| Parcel Peak | 1 | Magpie boss (only boss) | 5–8 min |
| Stamp side-paths | ~1 per route | Optional | optional |

**Flow:** Title → Stamp Square → route → deliver → Stamp Square → … → Parcel Peak → Ending.

```text
Stamp Square
   ├── Daisy Downs        (letter 1 — meadows)
   ├── Rose Ramble        (letter 2 — flower beds)
   ├── Clothespin Cliffs  (letter 3 — backyard lines)
   └── Tick-Tock Terrace  (letter 4 — dusk towers)
           └── Parcel Peak
                 └── Ending (by stamp count)
```

**Stages:** A few screens wide. At most two tall. Route 1 opens empty so the player can try jump and glide.

**Teaching:** After Route 1, add at most one new move or enemy idea per route. Prefer layout over text.

---

## Core loop (per level)

1. **Find the letter** — mid-route, not at the start.
2. **Deliver it** — reach the recipient. Short line or two + emote. Route clears. Back to Stamp Square.

**While carrying:** movement unchanged. Letter stays with you until delivery. Hits do not drop it. No drop button.

### Optional stamps
One rare stamp per route, off the main path. Not required. Stamp count changes the ending (shed → office → fancy sorting place).

---

## Player verbs

### Move and jump
Walk, jump, coyote time.

### Mucus Glide
Hold float in air to fall slowly and drift a short way. For recovery and pits — not free flight.

### Shell Bump
Short forward bump on ground (brief air bump OK). Hits small enemies, soft blocks, secret bells.

### Stomp
Land on most enemy heads to defeat or stun them. Small bounce.

**Do not add:** tongue combat, item menus, coin shops, many power-ups.

---

## Movement kit

| Feature | Ship? | Notes |
|---------|-------|-------|
| Coyote time | Yes | |
| Double jump | No | |
| Floating (Mucus Glide) | Yes | |
| Drop-through platforms | Yes | |
| Moving / solid platforms | Sparingly | |
| Ladders | One route max | Clothespin Cliffs |
| Dash | No | |
| Wall jump | No | |
| Run | No | |

---

## Difficulty (no death)

Like Wario Land 2: you cannot die or game over from damage.

- **No HP. No hearts. No lives. No game over.**
- Enemy hit = knockback + short stun. Maybe bounce or brief flatten. Then continue.
- Pit / screen exit = soft recover to the nearest safe ledge (or bounce back). Not a restart.
- Challenge = delay, knockback into bad spots, boss patterns — not run failure.
- Most enemies patrol. Few chase.
- Combat is optional when a path exists around it.
- Short dialogue only.

---

## Enemies and bosses

Real garden threats. Four types max:

1. **Ant** — ground patrol. Stomp or bump.
2. **Beetle** — slow ground foe. Stomp or bump.
3. **Spider** — hangs or drops from above. Bump or glide past.
4. **Wasp** — slow flyer. Glide under or ignore. Midgame+.

**One boss:** Magpie at Parcel Peak. Steals shiny mail.

- Peck / dive can knock you off the platform.
- No death. Soft recover to the arena start.
- Boss fight **restarts** (hit count resets).
- Win by landing enough hits in one clean attempt.


No midgame bosses.

---

## Presentation

- **Color:** One mood per route (Daisy Downs green, Rose Ramble pink, Clothespin Cliffs sky blue, Tick-Tock Terrace amber).
- **Music:** Hub jingle, one theme per route, delivery sting, boss track.
- **Title:** Logo + start. No lore scroll.
- **Ending:** Changes with stamp count. Same credits length.

---

## Non-goals

- Metroidvania backtracking.
- Shops, tolls, coin gambling, score chase.
- Power-up sets, transformations, inventory screens.
- Slime trail physics.
- 15+ stages, secret worlds, New Game+.
- Hard mode as a day-one need.
- Midgame bosses.
- Death, HP, lives, game over, death checkpoints.

---

## GB Studio constraints

- Use platformer scenes. Enable only the moves you ship.
- Letters, recipients, stamps = actors, triggers, variables. No death/respawn state machine.
- Carry = a flag. Hits never drop the letter. No second movement mode.
- Hits = knockback / stun scripts. Pits = trigger warp or bounce to safe ground.
- Shell Bump = short scripted lunge or hit window.
- Keep actor count low. Split long routes across scenes.
- Save at the hub is enough.

---

## Fun checklist

1. Finding the letter feels good.
2. Delivery is a clear beat.
3. Shell Bump is useful and a bit funny.
4. Mucus Glide helps with pits and mistimed jumps.
5. Stamps reward looking around.
6. Hits only slow you down.
7. The snail always feels like a snail.
