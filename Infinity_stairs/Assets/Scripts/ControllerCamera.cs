using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ControllerCamera : MonoBehaviour
{
    public GameObject Target; // ��ǥ Ÿ��
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(Target != null)
        {
            // ī�޶� Ÿ���� ��ġ�� �̵�
            this.transform.position = Target.transform.position + new Vector3(0, 2f, -10f);
        }
    }
}
