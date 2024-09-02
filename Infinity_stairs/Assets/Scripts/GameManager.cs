using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    public bool Game_Start = false; // 게임 시작 체크
    public float Current_Time = 0.0f; // 현재 남은 시간
    public float Destination_Time = 10.0f; // 전체 시간
    public float Add_Time_Flow = 0.003f; // 감소 시간
    public Slider slider; /// 시간 타이머
    public TextMeshProUGUI text; // 점수 텍스트
    public int score = 0; // 점수
    public GameObject Character; // 캐릭터
    public Transform Platform_Parents; // 정리를 위한 발판들의 부모 오브젝트
    public GameObject Platform; //
    private List<GameObject> Platform_List = new List<GameObject>(); // 발판 리스트
    private List<int> Platform_Check_List = new List<int>(); // 발판의 위치 리스트, 왼쪽은 0, 오른쪽 1

    // Start is called before the first frame update
    void Start()
    {
        Data_Load();
        Init();
    }

    // Update is called once per frame
    void Update()
    {
        if(Game_Start)
        {
            if(Input.GetKeyDown(KeyCode.RightArrow))
            {
                Check_Platform(Character_Pos_Idx, 1);
            }
            else if (Input.GetKeyDown(KeyCode.LeftArrow))
            {
                Check_Platform(Character_Pos_Idx, -1);
            }

            Destination_Time = Destination_Time - Add_Time_Flow;
            Current_Time = Current_Time - Time.deltaTime;

            slider.value = Current_Time / Destination_Time;

            if(Current_Time < 0f)
            {
                Result();
            }
        }
        else
        {
            if(Input.GetKeyDown(KeyCode.Space))
            {
                Init();
            }
        }
    }

    public void Data_Load() // 데이터 로드로 게임 오브젝트를 생성
    {
        for(int i =0; i < 20; i++)
        {
            GameObject t_Obj = Instantiate(Platform, Vector3.zero, Quaternion.identity);
            t_Obj.transform.parent = Platform_Parents;
            Platform_List.Add(t_Obj);
            Platform_Check_List.Add(0);
        }

        Platform.SetActive(false);
    }

    private int Pos_Idx = 0; // 발판의 마지막 위치 저장
    private int Character_Pos_Idx = 0; // 캐릭터의 위치

    public void Init() // 캐릭터의 위치 초기화, 발판 위치 초기화
    {
        Character.transform.position = Vector3.zero;

        for(Pos_Idx = 0; Pos_Idx < 20;)
        {
            Next_Platform(Pos_Idx);
        }

        Destination_Time = 10.0f;
        Current_Time = Destination_Time;

        Character_Pos_Idx = 0;
        score = 0;
        text.text = score.ToString();
        Game_Start = true;
    }

    public void Next_Platform(int idx)
    {
        int pos = Random.Range(0, 2);
        Debug.Log("블럭 생성 Pos_Idx : " + Pos_Idx);

        if(idx == 0) // 첫번째 발판의 경우
        {
            Platform_List[idx].transform.position = new Vector3( 0, -0.5f, 0);
        }
        else
        {
            if(Pos_Idx < 20)
            {
                if (pos == 0) // 왼쪽 발판의 경우
                {
                    Platform_Check_List[idx - 1] = pos - 1;
                    Platform_List[idx].transform.position = Platform_List[idx - 1].transform.position + new Vector3(-1, 0.5f, 0);
                }
                else // 오른쪽 발판의 경우
                {
                    Platform_Check_List[idx - 1] = pos;
                    Platform_List[idx].transform.position = Platform_List[idx - 1].transform.position + new Vector3(1f, 0.5f, 0);
                }
            }
            else
            {
                if (pos == 0) // 왼쪽 발판의 경우
                {
                    if(idx % 20 == 0)
                    {
                        Platform_Check_List[20 - 1] = pos - 1;
                        Platform_List[idx % 20].transform.position = Platform_List[20 - 1].transform.position + new Vector3(-1, 0.5f, 0);
                    }
                    else
                    {
                        Platform_Check_List[idx % 20 - 1] = pos - 1;
                        Platform_List[idx % 20].transform.position = Platform_List[idx % 20 - 1].transform.position + new Vector3(-1, 0.5f, 0);
                    }
                }
                else // 오른쪽 발판의 경우
                {
                    if (idx % 20 == 0)
                    {
                        Platform_Check_List[20 - 1] = pos;
                        Platform_List[idx % 20].transform.position = Platform_List[20 - 1].transform.position + new Vector3(1, 0.5f, 0);
                    }
                    else
                    {
                        Platform_Check_List[idx % 20 - 1] = pos;
                        Platform_List[idx % 20].transform.position = Platform_List[idx % 20 - 1].transform.position + new Vector3(1, 0.5f, 0);
                    }
                }
            }
        }
        score++;
        text.text = score.ToString();
        Pos_Idx++;
    }

    void Check_Platform(int idx, int direction)
    {
        Debug.Log("Idx : " + idx % 20 + " /Platform : " + Platform_Check_List[idx % 20] + " /Direction :" + direction);
        if (Platform_Check_List[idx % 20] == direction)
        {
            Character_Pos_Idx++;
            Character.transform.position = Platform_List[Character_Pos_Idx % 20].transform.position + new Vector3(0f, 0.5f, 0f);
            Current_Time = Destination_Time;
            Next_Platform(Pos_Idx);
        }
        else
        {
            Character.transform.position = Platform_List[Character_Pos_Idx % 20].transform.position + new Vector3(direction, 0.5f, 0f);
            Result();
        }
    }

    public void Result()
    {
        Debug.Log("Game Over");
        Game_Start = false;
    }
}
