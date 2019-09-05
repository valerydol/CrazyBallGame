using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 * move Ball with buttons on keyboard
 */

public class Moving : MonoBehaviour {

    public float speed;
    public Rigidbody rigidbody;
    public Vector3 movement;
    public float courentDeltaTime;
    public static float moveHorizontal;
    public static float moveVertical;
    // Use this for initialization
    void Start () {
          this.rigidbody = GetComponent<Rigidbody>();
        speed = 30.0f;
    }
	
	  
    void FixedUpdate()
    {

        moveHorizontal = Input.GetAxis("Horizontal");
        moveVertical = Input.GetAxis("Vertical");

      
        movement = new Vector3(moveHorizontal * (-1.0f), 0.0f, moveVertical * (-1.0f));
        courentDeltaTime = Time.deltaTime;
        rigidbody.AddForce(movement * speed * Time.deltaTime);
    }
}
