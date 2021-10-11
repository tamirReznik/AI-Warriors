
# AI-Warriors
AI Team Deathmatch 

Map's legend

- ![](images/Aspose.Words.fd7e835c-2be8-428c-bdca-3eee88cf217d.001.png) - Medical Care

- ![](images/Aspose.Words.fd7e835c-2be8-428c-bdca-3eee88cf217d.002.png) - Ammo Station

- ![](images/Aspose.Words.fd7e835c-2be8-428c-bdca-3eee88cf217d.003.png) - Barrier

- ![](images/Aspose.Words.fd7e835c-2be8-428c-bdca-3eee88cf217d.004.png) - Warrior

**Right click** - 

  ![](images/Aspose.Words.fd7e835c-2be8-428c-bdca-3eee88cf217d.005.png)

**Operating Instructions**

**Start/Stop Battle**: AI warriors start to fight. It can be stopped at any time.

**Show/Hide Security Map** – show Security map in real-time.

**New Game** – start a new game without exit the current window.

- **To see the current state (hp and ammo) of all warriors - left click inside the window, and the updated data will appear in the console.**

Warrior States

- Lack of hp (injured) -> hp less then hp threshold
- Lack of ammo
- Assist teammate
- Attack

Warrior Character

- Each Warrior have a unique property of hp threshold that determines how much he can stay in the battle and get hit before starting to look for medical help.
- Each Warrior have a unique property of courage that determine how safe his path would be towards the target (coefficient of the security map weight)

![](images/%E2%80%8F%E2%80%8FDecisionTree.png)

**In every state, the Warrior runs BFS on his current room to check if he close to Teammate, Enemy, Ammo, Medical Station.**
  - In case the Warrior close to his teammate, he will check if he can help him with ammo and first aid or get from him ammo and first aid if needed.
  - In Incase warrior close to Ammo bunker, he automatically reloads a quarter of the maximum ammo possible to carry. If he already has maximum, then nothing happens.
  - In Incase warrior close to Medical Station, he automatically gets max health points.
  - In case warrior found an enemy in his room, he will attack him (if ammo not empty) and recalculate his path – if he has enough ammo, hp and his teammate is ok he will search for the best spot in the room (depends on security map and distance from the enemy) to fight. Otherwise, he will prefer to increase his health, reload or help his teammate.

**Main Algorithms**

- **findClosestTarget()** – **BFS** That gets the index of the target room as an argument and finds a path to the room.

- **findPathBetweenRooms()** – **A-star** that gets the target room's index as an argument and finds a path to the room.
  - H – security map value of the nodes in the path (with unique courage coefficient of the warrior)
  - G – number of nodes from the root node

- **findSafeSpotInRoom()** – **BFS** that search for a safe spot inside the room, safe spot considers having low value in security map and far from the enemy.

- **findPathInsideRoom()** – **A-star** That gets target node as argument and finds a path to this node.
  - Unlike **findPathBetweenRooms()** in **findPathInsideRoom()**, the search gives up nodes that are not in the room area and does not put them in the priority queue or the vector of grays nodes.

- **closeRadar()** – **BFS** that search for enemy in warrior room if enemy found in the room warrior will shoot to kill (if he has ammo)


