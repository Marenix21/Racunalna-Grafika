using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DayNightAnimation : MonoBehaviour
{

    public void TurnOn()
    {
        this.GetComponent<Animator>().SetBool("Start", true);
    }



    public void TurnOff()
    {
        this.GetComponent<Animator>().SetBool("Start", false);
    }
}
