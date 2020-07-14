# WorldBorder

World border mod for [ElementZero](https://github.com/Element-0/ElementZero).<br><br>
You need to set global world spawn with /setworldspawn command.

Config
-------------
```
  WolrdBorder:
    enabled: true
    reachBorderMessage: You've reached world border!
    messageBorderApproach: You will reach border at %coord% pos in %number% blocks!
    informBefore: 10
    overworld:
      minX: -1000
      minZ: -1000
      maxX: 1000
      maxZ: 1000
    nether:
      minX: -125
      minZ: -125
      maxX: 125
      maxZ: 125
    end:
      minX: -1000
      minZ: -1000
      maxX: 1000
      maxZ: 1000
```
