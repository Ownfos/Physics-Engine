# Physics-Engine
This project implements collision response of 2d rigid body.
Usable shapes are polygon and circle.


Control

  Number keys
  
    1 : Dragging mode. You can pull or move objects around by clicking and dragging them.
    2 : Polygon mode. You can set custom convex polygon's vertices with left mouse click, and create it with right mouse click.
    3 : Circle mode. You can create random or uniform size circles with left mouse click.
    4 : Box mode. You can create random or uniform size boxes with left mouse click.
    5 : Spring mode. You can connect two objects with spring or hinge-joint. Click one object and release mouse button on other object.
    6 : Delete spring mode. You can disconnect every spring/hinge-joint connected to the clicked object.
    7 : Destroy mode. You can destroy the clicked object.
    
  Alphabets
  
    S : Show collision. When enabled, every contact and it's collision normal will be drawn.
    R : Show spring. When enabled, every spring and hinge-joint will be drawn.
    G : Gravity. Disable it to place objects at desired position.
    C : Perfect correction. When enabled, collision response will seperate each object completely. Using this might cause 'shaking'.
    J : Is joint. When enabled, Spring mode will create hinge-joint instead of spring.
    U : Uniform size. Decides whether Circle and Box mode will create objects with same size.
    M : Damping. When enabled, objects will automatically slow down in terms of linear and angular velocity. Use it to stabilize joints.
    D : Drag mode. 'pull' mode will apply some force and drag it smoothly, while 'move' mode will change the position directly.
