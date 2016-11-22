/**@<pseudoassembly.c>::**/

int labelcounter = 1;
int gofalse (int label)
{
    fprintf(object, "tjz .L%d\n", label);
    return label;
}

int jump(int label)
{
    fprintf(object, "tjmp .L%d\n", label);
    return label;
}

int mklabel (int label)
{
    frprintf(object, ".L %d:\n", label);
    return label;
}
