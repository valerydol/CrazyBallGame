using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/* move video object after camera*/
public class PlaneFollow : MonoBehaviour {



    [SerializeField] Transform target;
    Rigidbody body;


    public Vector3 parentVector;
    public Transform parentTransform;
    // Use this for initialization
    void Start () {


        body = GetComponent<Rigidbody>();

        parentVector = GameObject.Find("Ball").transform.position;

      // StartCoroutine(MoveOverSeconds(gameObject, parentVector, 5f));

    }
	
	// Update is called once per frame
	void Update () {

        if (target.position.x > (-38.0f))
        {
            transform.position = new Vector3(target.position.x, transform.position.y, transform.position.z);
            return;
        }
       
        if (target.position.x < (-38.0f) && target.position.z < -1.25f)
        {
            transform.position = new Vector3(transform.position.x, transform.position.y, target.position.z);
            return;
        }
        if(target.position.x < (-38.0f) && target.position.z > 0.0f)
        {
            transform.position = new Vector3(transform.position.x, transform.position.y, -1.25f);
            return;
        }

    }





    public IEnumerator MoveOverSeconds(GameObject objectToMove, Vector3 end, float seconds)
    {
        bool flag = true;
        float elapsedTime = 0;
        Vector3 startingPos = objectToMove.transform.position;
        while (elapsedTime < seconds)
        {
            float dist = Vector3.Distance(end, objectToMove.transform.position);
         


            objectToMove.transform.position = Vector3.Lerp(startingPos, end, (elapsedTime / seconds));
            elapsedTime += Time.deltaTime;

            
            GameObject marker = GameObject.Find("marker_1");
            Debug.Log(marker.transform.position.z);
            if (dist < 9.0f )
            {
                flag = false;
                break;
            }
            yield return new WaitForEndOfFrame();
        }
        if (flag)
            objectToMove.transform.position = end;
    }

}
