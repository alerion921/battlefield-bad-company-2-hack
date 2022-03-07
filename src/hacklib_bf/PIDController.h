#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H


enum class ControlType
{
    P,
    PI,
    ClassicPID,
    PessenIntegralRule,
    SomeOvershoot,
    NoOvershoot
};

template <class T, typename TTime = double, typename TConst = double>
class PIDController
{
public:
    void tuneManual(TConst p, TConst i, TConst d)
    {
        m_p = p;
        m_i = i;
        m_d = d;
    }

    void tuneZieglerNichols(TConst ultimateGain, TConst oscillationPeriod, ControlType type)
    {
        switch (type)
        {
        case ControlType::P:
            m_p = 0.5*ultimateGain;
            m_i = 0;
            m_d = 0;
            break;
        case ControlType::PI:
            m_p = 0.45*ultimateGain;
            m_i = (1.2*m_p) / oscillationPeriod;
            m_d = 0;
            break;
        case ControlType::ClassicPID:
            m_p = 0.6*ultimateGain;
            m_i = (2*m_p) / oscillationPeriod;
            m_d = (m_p*oscillationPeriod) / 8;
            break;
        case ControlType::PessenIntegralRule:
            m_p = 0.7*ultimateGain;
            m_i = (2.5*m_p) / oscillationPeriod;
            m_d = (m_p*oscillationPeriod) * 0.15;
            break;
        case ControlType::SomeOvershoot:
            m_p = 0.33*ultimateGain;
            m_i = (2*m_p) / oscillationPeriod;
            m_d = (m_p*oscillationPeriod) / 3;
            break;
        case ControlType::NoOvershoot:
            m_p = 0.2*ultimateGain;
            m_i = (2*m_p) / oscillationPeriod;
            m_d = (m_p*oscillationPeriod) / 3;
            break;
        }
    }

    T step(TTime dt, T setpoint, T measured)
    {
        T error = setpoint - measured;
        m_integral = m_integral + (T)(error*dt);
        T derivative = (T)((error - m_prevError)/dt);
        m_prevError = error;
        return (T)(m_p*error + m_i*m_integral + m_d*derivative);
    }

private:
    TConst m_p = 0;
    TConst m_i = 0;
    TConst m_d = 0;
    T m_prevError = T();
    T m_integral = T();

};

#endif