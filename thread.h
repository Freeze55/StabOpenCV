#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QObject>


class Thread : public QObject
{   Q_OBJECT
public:
  Thread();

public slots:
  void Stabilize(QString inputPath,QString outputPath,int outputFps, bool Type_Pass_Stabilizer, bool quietMode );

signals:
  void restart();




};
#endif // THREAD_H
