**Route Planning Project**

Author@Yuning Zhang  Time@2023.5.15

**introduction**

This project based on leeds.map file is expected to offer 3 route plans: shortest path, quickest path and the path passing by several ordered nodes and simultaneously has some other useful functions.

Firstly, it can dynamically read links, nodes, and other labels in the map file so it's efficient to store complex data. Then, I choose adjacency List to organize the distance information to quickly perform the dijkstra algorithm.

The project has 6 main functions: (1)find path with min-distance, (2)find path with min-time, (3)find shortest path with constraints that require passing some points by order, (4)edit or add link's attributes, (5)search nodes when visualizing, (6) add link.

Also, I devoted to making the GUI user-friendly, and user could  nearly enter commands only by GUI instead of the black console.

**usage**

The 'homepage' of the project contains vivid map in the left and menu with 6 options in the right.

Enter 1~6, then it would do correspond functions. 'Esc' is the back key for every function, and the right click of mouse could often clear marks or highlights on the map.

For '1.Min Distance', you can click two points, regarded as start and end point, to find the shortest path marked in green. The information of chosen points and min distance would be shown in the right. It's necessary to mention that the path nodes and distance would be recorded in the "routePlanNearestPath.txt" given more detailed information. You can click right of mouse to clear marked path and choose other nodes. 

For '2.Min Time', the workflow of choosing nodes is similar to Min Distance. It's known that t=d/v so speed is required to planning quickest path. Speed(actually max speed) can be derived from either default number or input number. After researching the Open Street Map files, it's make sense that longer links or ways may have greater (max) speed. So I set the default speed by the rules below: if the link's length greater than 60, then the speed is assigned as 30. If thh length is between 20 and 60, the speed is 20. If the length is less than 20, the speed would be 5. At the same time, the speed can be added or changed by user, and the default speed would be covered by user's value. The visualization method is same as the '1.Min Distance', and detailed information also can be check in "routePlanQuickPath.txt".

For '3.Pass Ordered Nodes', it would allow you to add the restriction of path by choosing the ordered nodes within the path. You can choose several nodes(>=2) and enter the return key to plan path with min-distance.

For '4.Edit Link', it's aimed to edit the attributes of the link,such as veg, poi, speed and so on. Initially, click 2 nodes to choose an existing link, then click the text box to enter in this format: "${mode} ${value}". The mode is related to several attributes for user to changes or add, and the value is to assign the chosen attribute. If editing Successfully, GUI would remind you of save it into a new map file or abort it. If  you'd like to save it, please enter left-control key. If not, enter 'Esc' to escape. If you want to edit another link, it's required to click right button and 'esc' key and then enter again. Remember that every editing operation would be recorded in the 'editLog.txt' so that you can check what you change or add conveniently.

For '5.Search Node', it's a tool to help user locate the points in map by raw ID of nodes. Just input the raw ID of nodes within the bounding of map, the point with this unique raw ID would be filled by black on the map. 

For '6.Add Link', you can choose both linked nodes or isolated points to add a link between them. If you are pleasant for the added link, you can save it by entering the left-control key too.

**How to launch**

Initially, compiling and linking source files by Makefile is needed: `make all`.

Then it could be launched by the commend `./main mapFile.map`.

Please make sure all the source file are in the same directory and check the path and access of map file. 

** **

