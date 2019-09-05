using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Arraw : MonoBehaviour {

    
    // Use this for initialization
    GameObject left;
    GameObject right;
    GameObject up;
    GameObject down;
    GameObject Ball;
    VirtualMoving.MOVE_TO go_to;
 
	void Start () {

        left = GameObject.Find("left");
        right = GameObject.Find("right");
        up = GameObject.Find("up");
        down = GameObject.Find("down");

        left.SetActive(false);
        right.SetActive(false);
        up.SetActive(false);
        down.SetActive(false);

        Ball = GameObject.Find("Ball");

        go_to = VirtualMoving.MOVE_TO.STOP;
    }
	
	// Update is called once per frame
	void Update () {
        left.SetActive(false);
        right.SetActive(false);
        up.SetActive(false);
        down.SetActive(false);

        #region MOVE SCRIPT WITH ARRAWS
        // float hor = Moving.moveHorizontal;
        // float ver = Moving.moveVertical;
        /*
           if (hor!= 0)
           {
               if (hor == -1)
               {
                   left.SetActive(true);
                   return;
               }
               right.SetActive(true);
               return;

           }
           if(ver == 1)
           {
               up.SetActive(true);
               return;
           }
           down.SetActive(true);
           */

        #endregion



        #region Virtual MoveScript

        go_to = (Ball.GetComponent<VirtualMoving>()).goTo;

        
        if(go_to == VirtualMoving.MOVE_TO.LEFT)
        {
            left.SetActive(true);
            return;
        }
        if (go_to == VirtualMoving.MOVE_TO.RIGHT)
        {
            right.SetActive(true);
            return;
        }
        if (go_to == VirtualMoving.MOVE_TO.UP)
        {
            up.SetActive(true);
            return;
        }
        if (go_to == VirtualMoving.MOVE_TO.DOWN)
        {
            down.SetActive(true);
            return;
        }

        #endregion

    }
}
