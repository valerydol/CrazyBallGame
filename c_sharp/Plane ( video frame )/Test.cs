using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;
using System.Text;


[StructLayout(LayoutKind.Sequential, Size = 12)]
public struct CvVector3D
{
    public float X, Y ,Z;
}
internal static class OpenCVInterop
{

    /*
    [DllImport("OpenCvProject")]
    internal unsafe static extern void getTranslationVectorsAndSize(CvVector3D* outTranslationVector, ref int outTransVectorSize); // , ref float maxOutFacesCount, ref float outY);
    */

    [DllImport("OpenCvProject")]
    internal unsafe static extern void getRotationVectorsAndSize(CvVector3D* outRotationVector , ref int outRotVectorSize); // , ref float maxOutFacesCount, ref float outY);

}





public class Test : MonoBehaviour
{

 
    [DllImport("OpenCvProject", EntryPoint = "?Initializer@ArucoBoard@BoardCalibration@@SAXXZ")]
    public static extern void Initializer();

    [DllImport("OpenCvProject", EntryPoint = "?loadCameraCalibration@ArucoBoard@BoardCalibration@@SA_NXZ")]
    public static extern bool loadCameraCalibration();

    [DllImport("OpenCvProject", EntryPoint = "?openCamera@ArucoBoard@BoardCalibration@@SA_NXZ")]
    public static extern bool openCamera();

    [DllImport("OpenCvProject", EntryPoint = "?saveData@ArucoBoard@BoardCalibration@@SAXXZ")]
    public static extern void saveData();

    [DllImport("OpenCvProject", EntryPoint = "?startWebcamMonitoring@ArucoBoard@BoardCalibration@@SAPEAEXZ")]
    public static extern byte[] startWebcamMonitoring();

    [DllImport("OpenCvProject", EntryPoint = "?startWebcamMonitoringWithoutVideoStream@ArucoBoard@BoardCalibration@@SAXXZ")]
    public static extern void startWebcamMonitoringWithoutVideoStream();

   
    Material m;

    public static Queue<Vector3> rotationQueue = new Queue<Vector3>(); 

   // public static Queue<Vector3> transformeQueue = new Queue<Vector3>();

    CvVector3D[] _rotationVec  = new CvVector3D[50];
    int _rotationVectorSize = 0;

    
    public static float moveHorizontal = 0.0f;
    public static float moveVertical = 0.0f;


    public enum MOVE_TO { LEFT , RIGHT, UP ,DOWN , STOP} ;

    public  MOVE_TO moveBallTo = MOVE_TO.STOP;

    //NOT IN USE
    // public CvVector3D[] _translationVec;
    // int _transformVectorSize = 0;


    // Use this for initialization
    void Start()
    {
       
        m = new Material(Shader.Find("Diffuse"));
        Initializer();
        bool ans_1 = loadCameraCalibration();
        bool ans_2 = openCamera();


       // Debug.Log("BoardTest : loadCameraCalibration  => " + ans_1 + "  open camera " + ans_2);
      
        // NOT IN USE
      //  _translationVec = new CvVector3D[50];
  
    }

    // Update is called once per frame
    void Update()
    {
      
     
        #region VideoConvertion Code WITH VIDEO
        
         byte[] imgData = startWebcamMonitoring();
         Texture2D tex = new Texture2D(640, 480, TextureFormat.BGRA32, false);

         tex.LoadRawTextureData(imgData);
         tex.Apply();

         m.mainTexture = tex;
         this.GetComponent<Renderer>().material = m;

        #endregion

        #region video monitoring without video

        // no video , mountains only
        /* startWebcamMonitoringWithoutVideoStream(); */


        #endregion


        //update rotation vector size 
        updateRotationQueue();
        
        /* transphorme not in use */
        // updateTransformationQueue();


    }

    void updateRotationQueue()
    {
        int size = 0;
        unsafe
        {
            fixed (CvVector3D* outRotationVector =  _rotationVec )
            {
                OpenCVInterop.getRotationVectorsAndSize(outRotationVector,  ref size);
            }
        }
        _rotationVectorSize = size;
        if (size > 0)
        {
            insertInRotationQueue();
        }
            

    }



    void insertInRotationQueue()
    {
        for (int i = 0; i < _rotationVectorSize; i++)
        {
          VirtualMoving.vecX = new Vector3(_rotationVec[i].X, 0.0f, 0.0f);
          VirtualMoving.vecY = new Vector3(0.0f, 0.0f, _rotationVec[i].Z);
         
            // call GC , we must free mamory from c++
            GC.Collect();
            GC.WaitForPendingFinalizers();
        }
    }



    #region NOT IN USE
    /* 
     
    void insertTransformQueue()
    {
        for (int i = 0; i < _transformVectorSize; i++)
        {
            //   rotationQueue.Enqueue(new Vector3(_rotationVec[i].X, _rotationVec[i].Y, 0.0f));

            // rotationQueue.Enqueue(new Vector3(_rotationVec[i].X, _rotationVec[i].Y, _rotationVec[i].Z));
            Debug.Log( " transformation : " + new Vector3(_translationVec[i].X, _translationVec[i].Y, _translationVec[i].Z));

            //  float angle_0_1_0 = Vector3.Angle(new Vector3(_rotationVec[i].X, _rotationVec[i].Y, _rotationVec[i].Z), vec_0_1_0); // <<  --- rotation left right
        }
    }

    */

    /*
    void updateTransformationQueue()
    {
        int size = 0;
        unsafe
        {
            fixed (CvVector3D* outTransformVector = _translationVec)
            {
                OpenCVInterop.getTranslationVectorsAndSize(outTransformVector, ref size);

            }
           //  Debug.Log(size);
        }

        _transformVectorSize = size;
        if (size > 0)
           insertTransformQueue();
    }
    */
    #endregion

}

