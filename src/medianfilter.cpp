#include "medianfilter.hpp"

MedianFilter::MedianFilter(int width, int height, uchar size)
{
    cv::Mat frame = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));
    MedianFilter(frame, size);
}

MedianFilter::MedianFilter(cv::Mat frame, uchar size)
{
    if (size < 6)
        size = 6;
    mFrameWidth = static_cast<uint>(frame.cols);
    mFrameHeight = static_cast<uint>(frame.rows);
    
    mVectorIterator = 0;
    mSize = size;
    mHalfSize = static_cast<uchar>(size / 2);
    std::vector<uchar> clean(size, 5);
    uint len = static_cast<uint>(mFrameWidth * mFrameHeight);
    for (uint i = 0; i < len; i++)
    {
        vectorOfValues.push_back(clean);
        vectorOfNext.push_back(clean);
        vectorOfPrev.push_back(clean);
    }
    initMedian(frame);
}

MedianFilter::~MedianFilter()
{}

void MedianFilter::initMedian(cv::Mat frame)
{
    //localCopy = frame;
    for (uint y = 0; y < mFrameHeight; y++)
    {
        for (uint x = 0; x < mFrameWidth; x++)
        {
            uint i = x + y * mFrameWidth;

            for (uint c = 0; c < mSize; c++)
            {
                vectorOfValues[i][c] =
                    static_cast<uchar>(frame.at<uchar>(cv::Point(static_cast<int>(x), static_cast<int>(y))));
                vectorOfNext[i][c] = static_cast<uchar>(c + 1);
                vectorOfPrev[i][c] = static_cast<uchar>(c - 1);
            }

            vectorOfValues[i][0] = (frame.at<uchar>(cv::Point(static_cast<int>(x), static_cast<int>(y))));

            vectorOfNext[i][mSize - 1] = 1;
            vectorOfPrev[i][1] = (mSize)-1;

            vectorOfNext[i][0] = 1;
            vectorOfPrev[i][0] = mSize - 1;

            // q1:
            // vectorOfNext[i][mSize] =  frame.at<uchar>(cv::Point(x,y)) ;
            // q3:
            // vectorOfPrev[i][mSize] = frame.at<uchar>(cv::Point(x,y)) ;
        }
    }
}

void MedianFilter::showIterator()
{
    int i = 0;
    for (std::vector<uchar>::iterator it = vectorOfValues[0].begin(); it != vectorOfValues[0].end(); it++)
    {
        std::cout << static_cast<int>(i) << '\t';
        i++;
    }
    std::cout << "\n";
}

void MedianFilter::printCollection(std::vector<uchar> &collection)
{
    for (int i = 0; i < collection.size() ; i++)
    {
        if(i == 0)
            std::cout << "\033[1;31m" <<  static_cast<int>(collection[i]) << "\033[0m" <<  '\t';
        else
            std::cout << static_cast<int>(collection[i]) << '\t';
    }
    std::cout << "\n";
}

void MedianFilter::showMedianNode(uint temp)
{
    if (temp > vectorOfValues.size() || temp > vectorOfNext.size() || temp > vectorOfPrev.size())
        return;
    printCollection(vectorOfValues[temp]);
    printCollection(vectorOfNext[temp]);
    printCollection(vectorOfPrev[temp]);
}

void MedianFilter::removeNode()
{
    mVectorIterator++;
    if (mVectorIterator >= mSize)
        mVectorIterator = 1;

    uint xy;
    //#pragma omp parallel for num_threads(2)  schedule(dynamic)

    for (xy = 0; xy < mFrameHeight * mFrameWidth; xy++)
    {
        uchar start = vectorOfNext[xy][0];
        uchar stop = vectorOfPrev[xy][0];

        // int val = frame.at<char>(Point(x,y));//15
        if (start == mVectorIterator) // usuwamy start
        {
            // przesuwamy start o jeden do przodu:
            uchar nextStart = vectorOfNext[xy][start];

            // sprawdzamy jaki był przed startem:
            uchar prevStart = vectorOfPrev[xy][start];

            // zmieniamy poprzedniego aby wskazywał na nowy start:
            vectorOfNext[xy][prevStart] = nextStart;

            // zmieniamy następnego aby wzkazywał start jako poprzedni:
            vectorOfPrev[xy][nextStart] = prevStart;

            // zmieniamy start:
            vectorOfNext[xy][0] = nextStart;

            // zerujemy wartosci w nastepnym i poprzednim usunietego pola:
            // vectorOfNext[xy][start] = 0;
            // vectorOfPrev[xy][start] = 0;

            // zmieniamy stop na poprzedni nowego startu:
            vectorOfPrev[xy][0] = vectorOfPrev[xy][nextStart];
        }
        else if (stop == mVectorIterator) // usuwamy stop:
        {
            // sprawszamy jaki był przed stopem:
            uchar nextStop = vectorOfPrev[xy][stop];

            // sprawdzamy jaki jest start: Bo po stopie jest start:
            uchar prevStop = vectorOfNext[xy][stop];

            // zmieniamy aby nowy stop wskazywał na start:
            vectorOfNext[xy][nextStop] = prevStop;
            // zmieniamy aby start wskazywał na nowy stop:
            vectorOfPrev[xy][prevStop] = nextStop;

            // zmieniamy stop:
            vectorOfPrev[xy][0] = nextStop;

            // zerujemy wartości poprzedniego stopu:
            // vectorOfNext[xy][stop] = 0;
            // vectorOfPrev[xy][stop] = 0;

            // zmieniamy start na następny nowego stopu:
            vectorOfNext[xy][0] = vectorOfNext[xy][nextStop];
        }
        else
        {
            // wyznaczamy indexy next i prev:
            uchar next = vectorOfNext[xy][mVectorIterator];
            uchar prev = vectorOfPrev[xy][mVectorIterator];
            // następny poprzedniego wskazuje na następnego:
            vectorOfNext[xy][prev] = next;
            // poprezedni następnego wskazuje na poprzedniego:
            vectorOfPrev[xy][next] = prev;
            // zerujemy wartości:
            // vectorOfNext[xy][ mVectorIterator ] = 0;
            // vectorOfPrev[i][ mVectorIterator ] = 0;
        }
    }
}

void MedianFilter::addImage(cv::Mat frame)
{
    // RemoveNode();
    uint x, y;
    //#pragma omp parallel for private (x) num_threads(2) schedule(dynamic)
    for (y = 0; y < mFrameHeight; y++)
    {
        for (x = 0; x < mFrameWidth; x++)
        {
            uint i = x + y * mFrameWidth;
            uchar start = vectorOfNext[i][0];
            uchar stop = vectorOfPrev[i][0];
            uchar val = frame.at<uchar>(cv::Point(static_cast<int>(x), static_cast<int>(y)));
            uint insertRowValue = 0;
            uchar nextRow = 0;
            uchar median = vectorOfValues[i][0];

            if (val > median) // jezeli val jest wiekszy od mediany: szukaj od góry
            {
                if (val >= vectorOfValues[i][stop])
                {
                    // nowy stop to aktualna pozycja:
                    vectorOfPrev[i][0] = mVectorIterator;

                    // następna pozycja wskazuje na start:
                    vectorOfNext[i][mVectorIterator] = start;
                    // poprzednia pozycja wskazuje na starego stopa:
                    vectorOfPrev[i][mVectorIterator] = stop;
                    // nastepna starego stopa wskazuje na aktualna pozycję:
                    vectorOfNext[i][stop] = mVectorIterator;
                    // poprzednia starego startu wstazuje na aktualną pozycję:
                    vectorOfPrev[i][start] = mVectorIterator;
                    // wpisz wartość aktualnej pozycji:
                    vectorOfValues[i][mVectorIterator] = val;
                }
                else
                {
                    uchar insertRow = 0;
                    nextRow = stop; // 1

                    for (uint c = (mSize); c > 0; c--)
                    {
                        insertRow = vectorOfPrev[i][nextRow];
                        insertRowValue = vectorOfValues[i][nextRow];

                        if (val < insertRowValue)
                        {
                            nextRow = insertRow;
                            break;
                        }
                        nextRow = insertRow;
                    }

                    insertRow = nextRow;

                    uchar nextinsertROW = vectorOfNext[i][insertRow];

                    vectorOfNext[i][mVectorIterator] = nextinsertROW;
                    vectorOfPrev[i][mVectorIterator] = insertRow;

                    vectorOfNext[i][insertRow] = mVectorIterator;
                    vectorOfPrev[i][nextinsertROW] = mVectorIterator;

                    vectorOfValues[i][mVectorIterator] = val;
                }
            }
            else if (val <= median) // jezeli val jest mniejszy od mediany: szukaj od dołu
            {
                if (val <= vectorOfValues[i][start])
                {
                    vectorOfNext[i][0] = mVectorIterator;

                    vectorOfNext[i][mVectorIterator] = start;

                    vectorOfPrev[i][mVectorIterator] = stop;

                    vectorOfNext[i][stop] = mVectorIterator;
                    vectorOfPrev[i][start] = mVectorIterator;

                    vectorOfValues[i][mVectorIterator] = val;
                }
                else
                {
                    uchar insertRow = 0;
                    nextRow = start;

                    for (int c = 0; c < (mSize); c++)
                    {
                        insertRow = vectorOfNext[i][nextRow];
                        insertRowValue = vectorOfValues[i][nextRow];

                        if (val <= insertRowValue)
                        {
                            break;
                        }
                        nextRow = insertRow;
                    }

                    insertRow = nextRow;
                    uchar previnsertRow = vectorOfPrev[i][insertRow];

                    vectorOfNext[i][mVectorIterator] = insertRow;
                    vectorOfPrev[i][mVectorIterator] = previnsertRow;

                    vectorOfPrev[i][insertRow] = mVectorIterator;
                    vectorOfNext[i][previnsertRow] = mVectorIterator;

                    vectorOfValues[i][mVectorIterator] = val;
                }
            }
        }
    }
}

void MedianFilter::getMedian(cv::Mat frame)
{
    // cv::Mat copy = frame.clone();
    uint x, y;
    //#pragma omp parallel for private(x) num_threads(2) schedule(dynamic)
    // shared(vectorOfValues,vectorOfNext,vectorOfPrev,frame)
    for (y = 0; y < mFrameHeight; y++)
    {
        for (x = 0; x < mFrameWidth; x++)
        {
            uint i = x + y * mFrameWidth;
            uint start = vectorOfNext[i][0];
            uint nextRow = start;
            uint insertRow = 0;
            for (uint c = 0; c <= mHalfSize; c++)
            {
                insertRow = vectorOfNext[i][nextRow];
                nextRow = insertRow;
            }
            frame.at<uchar>(cv::Point(static_cast<int>(x), static_cast<int>(y))) = vectorOfValues[i][insertRow];
            vectorOfValues[i][0] = vectorOfValues[i][insertRow];
        }
    }
}

cv::Mat MedianFilter::getMedian()
{
    //cv::Mat frame = localCopy.clone();
    cv::Mat frame = cv::Mat(mFrameHeight, mFrameWidth, CV_8UC1, cv::Scalar(0));
    uint x, y;
    //#pragma omp parallel for private(x) num_threads(2) schedule(dynamic)
    // shared(vectorOfValues,vectorOfNext,vectorOfPrev,frame)
    for (y = 0; y < mFrameHeight; y++)
    {
        for (x = 0; x < mFrameWidth; x++)
        {
            uint i = x + y * mFrameWidth;
            uchar start = vectorOfNext[i][0];
            uchar nextRow = start;
            uchar insertRow = 0;
            for (uint c = 0; c <= mHalfSize; c++)
            {
                insertRow = vectorOfNext[i][nextRow];
                nextRow = insertRow;
            }
            frame.at<uchar>(cv::Point(static_cast<int>(x), static_cast<int>(y))) = vectorOfValues[i][insertRow];
            vectorOfValues[i][0] = vectorOfValues[i][insertRow];
        }
    }
    return frame;
}

void MedianFilter::unitTestInit(cv::Mat M, uint interator)
{
    initMedian(M);
    showIterator();
    //ShowMedianNode(interator);
}

void MedianFilter::unitTestCheck(std::vector<uchar> V1, std::vector<uchar> N1, std::vector<uchar> P1)
{
    uint i = 0;
    for (std::vector<uchar>::iterator it = vectorOfValues[0].begin(); it != vectorOfValues[0].end(); ++it)
    {
        if (vectorOfValues[0][i] != V1[i])
        {
            printf("VAL:błąd na iteracji[%d], w vectorOfValues powinno być:%d\n zamiast:%d\n", i, V1[i], vectorOfValues[0][i]);
        }
        i++;
    }
    std::cout << '\n';

    i = 0;
    for (std::vector<uchar>::iterator it = vectorOfNext[0].begin(); it != vectorOfNext[0].end(); ++it)
    {
        if (vectorOfNext[0][i] != N1[i])
        {
            printf("vectorOfNext:błąd na iteracji[%d], w vectorOfValues powinno być:%d\n zamiast:%d\n", i, N1[i], vectorOfNext[0][i]);
        }
        i++;
    }
    std::cout << '\n';

    i = 0;
    for (std::vector<uchar>::iterator it = vectorOfPrev[0].begin(); it != vectorOfPrev[0].end(); ++it)
    {
        if (vectorOfPrev[0][i] != P1[i])
        {
            printf("vectorOfPrev:błąd na iteracji[%d], w vectorOfValues powinno być:%d\n zamiast:%d\n", i, P1[i], vectorOfPrev[0][i]);
        }
        i++;
    }
    std::cout << '\n';
}


