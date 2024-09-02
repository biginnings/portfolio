using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ControllerCamera : MonoBehaviour
{
    public GameObject Target; // 목표 타겟
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(Target != null)
        {
            // 카메라가 타겟의 위치로 이동
            this.transform.position = Target.transform.position + new Vector3(0, 2f, -10f);
        }
    }
}
