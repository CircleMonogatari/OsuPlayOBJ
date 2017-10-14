#ifndef CONFIG_NUM_H
#define CONFIG_NUM_H
#include <QPoint>
#include <QString>

class config_num
{
public:
    config_num(const char *path);



    QPoint offset;
    QString S_filepath;
    QString fileName;
    int file_count;
    int mark_poi;
    int mark_max;
    int volnum;
    int H_tree_num;
    int H_tree_max;
};

#endif // CONFIG_NUM_H
