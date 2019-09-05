using System.Collections;
using System.Collections.Generic;
using UnityEngine;


/*
 * virtual moving, add this to ball object 
 * this code move ball with paper  
 */
public class VirtualMoving : MonoBehaviour {

    public float speed;

    public static Vector3 vecX;
    public static Vector3 vecY;

    Vector3 vec_1_0_1 = new Vector3(1.0f, 0.0f, 1.0f);


    public enum MOVE_TO { LEFT, RIGHT, UP, DOWN, STOP };

    public MOVE_TO moveBallTo = MOVE_TO.STOP;

    public MOVE_TO goTo = MOVE_TO.STOP;

    MOVE_TO lastMove = MOVE_TO.STOP;

    public Rigidbody player;
  
    void Start()
    {
        this.player = GetComponent<Rigidbody>();
        speed = 50.0f;
    }



    void FixedUpdate()
    {


         float moveHorizontal = getAngleHorisontal_X();
         float moveVertical = getAngleVertical_Y();
          
        goTo = calculateBallSideMove(moveHorizontal, moveVertical);

        Vector3 movement = getMovmentVector(goTo);
        player.AddForce(movement * speed * Time.deltaTime);
        lastMove = goTo;

     //  Debug.Log(this.transform.position);

    }



    MOVE_TO calculateBallSideMove(float X, float Y)
    {

      //  Debug.Log("X " + X + "  Y :" + Y);
        if (X == 45.0f)
        {
            if (Y == 45.0f)
            {
                return MOVE_TO.LEFT;
            }
            else
            {
                return MOVE_TO.DOWN;
            }
        }

        if (Y == 45.0f)
        {
            return MOVE_TO.UP;
           
        }
        return MOVE_TO.RIGHT;
    }


    float getAngleHorisontal_X()
    {
        return Vector3.Angle(vecX, vec_1_0_1);
    }

    float getAngleVertical_Y()
    {
        return Vector3.Angle(vecY, vec_1_0_1);
    }




    Vector3 getMovmentVector( MOVE_TO goTo)
    {
        if (goTo == MOVE_TO.LEFT)
            return new Vector3(1.0f, 0.0f, 0.0f);
        if(goTo == MOVE_TO.RIGHT)
            return new Vector3(-1.0f, 0.0f, 0.0f);
        if(goTo == MOVE_TO.UP)
            return new Vector3(0.0f, 0.0f, -1.0f);
        return new Vector3(0.0f, 0.0f, 1.0f);

    }


    float speedIndicator()
    {
        if ((goTo == MOVE_TO.LEFT && lastMove == MOVE_TO.RIGHT)
            || (goTo == MOVE_TO.RIGHT && lastMove == MOVE_TO.LEFT) )
            return speed * 100.0f;
        return speed;
    }


}
