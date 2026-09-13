# Anniversary barrel consequence reference (2026-09-10)

Source: <https://www.youtube.com/watch?v=4IkkNZxW0MQ&t=8s>, requested interval 7:40-9:40.

Observed sequence from a locally inspected 360p copy:

- At 7:40 the player accepts the warehouse watch. Barrel Man tells the Hero to stand between the
  stacks and not move, then leaves.
- The girl warns the Hero; on entering the warehouse the game explicitly says the barrels are
  breakable but are someone else's property.
- The player destroys the stock while the red watch timer is active. A second review from a local
  720p copy shows green floor glow and `+1`/combat feedback after the final destruction, but those
  visuals are experience/combat feedback and are not independent proof of a visible gold object.
  The footage does not expose the coin model or a gold counter clearly enough to certify it.
- Around 9:15, the game reports: `The man returned to the warehouse, but you weren't there!`
- The unattended-warehouse bad deed notification follows. On the subsequent Barrel Man interaction
  he says `A fat lot of good you were`, followed by
  `Now my stock's damaged! And to think I trusted you with it`, then the threat line.
- This is the requested 7:40-9:40 gameplay evidence that destroying the barrels reaches the
  Barrel Man's scolding outcome, rather than his successful-watch thanks.
- The run therefore proves the broken-stock rebuke is shipped and visible, while also visibly
  entering it after the unattended-return condition. It does not isolate Anniversary's behavior for
  the distinct nearby-at-return plus broken-stock combination seen in the New Oakvale playtest.

Gold evidence boundary:

- The video proves the destructive encounter and scolding outcome, not the gold definition.
- TLC's final-barrel reward is instead instruction/data-proven: WatchBarrels inserts
  `OBJECT_GOLD_1` into the singular remaining barrel on the penultimate break; the decoded object
  links to a `CGoldDef` value of exactly 1; and `CTCContainerRewardHero::OnDie` releases the inserted
  reward as a world pickup when that final barrel is destroyed. See
  `final-barrel-gold-native-release-20260910.md` and the gold-definition audit.

New Oakvale compatibility decision:

- A nearby Hero with `BarrelBrokenPersistent=true` receives `TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN`
  instead of the contradictory thanks and good deed.
- This compatibility branch does not add the distinct unattended-warehouse bad deed; the first
  barrel break has already supplied the destruction deed through `WatchBarrels`.
- TLC's exact automatic-return byte behavior remains captured unchanged in
  `barrel_man_full_retail_bytes.json` and `barrel_man_judgment_retail_bytes.json`.

Installed TLC text-bank confirmation:

- `text.big` SHA-256 `531C4514F5597E6426C8A2F1BD3ED39B3DD1DC9A8D38982E2303BB19F8FF33FC`
  defines group `TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN` as IDs 13209, 13223, 13235 in order.
- Their localized lines are `A fat lot of good you were.`, `Now my stock is damaged! And to think I
  trusted you with it.`, and `If I find out it was you, well... you better hope I don't!`.
- Thus the Lua compatibility branch names the exact TLC group whose ordered content matches the
  requested video segment; it is neither an invented key nor a one-line approximation.
