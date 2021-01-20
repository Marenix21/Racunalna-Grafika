using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Animate : MonoBehaviour
{

    private IEnumerator coroutine;



    private IEnumerator StartAnimation()
    {
        yield return new WaitForSeconds(10f);
        this.GetComponent<Animator>().SetBool("LieDown", true);
    }



    public void WakeUp() 
    {
        coroutine = StartAnimation();
        StartCoroutine(coroutine);
    }
}
