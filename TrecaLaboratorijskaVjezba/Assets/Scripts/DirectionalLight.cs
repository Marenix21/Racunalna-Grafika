using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DirectionalLight : MonoBehaviour
{
    public void TurnOn()
    {
        this.GetComponent<Light>().intensity = 1;
    }



    public void TurnOff()
    {
        this.GetComponent<Light>().intensity = 0;
    }
}
