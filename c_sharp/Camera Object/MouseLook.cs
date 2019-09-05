using System.Collections;
using System.Collections.Generic;
using UnityEngine;


/*add to camera object*/


public class MouseLook : MonoBehaviour
{
    
    public float saveZ = 0;//roration 

    [SerializeField] Transform target;
    void Start()
    {

    }

    void Update()
    {

        transform.localEulerAngles = new Vector3(80f, 180f, 0);

        if (target.position.x > (-38.0f))
        {
            transform.position = new Vector3(target.position.x, transform.position.y, transform.position.z);
            return;
        }
        if(target.position.x < (-38.0f) && target.position.z > 0.0f)
        {
            saveZ = transform.position.z;
            return;
        }
        if (target.position.x < (-38.0f) && target.position.z <= -1.25f)
        {
            transform.position = new Vector3(transform.position.x, transform.position.y, saveZ+ target.position.z );
            return;
        }
        



    }
}
