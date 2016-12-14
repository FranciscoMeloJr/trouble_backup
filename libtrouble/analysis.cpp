#include <algorithm>

#include "analysis.h"

Analysis::Analysis()
{
        qDebug("Shape of my heart");
        v1 = {1, 1, 1, 4};
        v2 = {0, 1, 2, 3 };
        int max_size = v1.length() + v2.length();
        QVector<int> intersec(max_size);
        QVector<int> diff(max_size);

        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());

        qDebug() << v1 << v2;

        auto it1 = std::set_symmetric_difference(v1.begin(), v1.end(),
                                                 v2.begin(), v2.end(),
                                                 diff.begin());

        auto it2 = std::set_intersection(v1.begin(), v1.end(),
                                         v2.begin(), v2.end(),
                                         intersec.begin());

        diff.resize(it1 - diff.begin());
        intersec.resize(it2 - intersec.begin());


        qDebug() << "diff" << diff;
        qDebug() << "intersection" << intersec;


        QVector<bool> group_diff(v1.length());
        for (int i = 0; i < v1.count(); i++) {
            qDebug() << "metric" << i << "is same:" << (v1[i] == v2[i]);
            group_diff[i] = (v1[i] == v2[i]);
        }
        qDebug() << group_diff;

}
