#include "angle.hpp"
#include "fixed_point.hpp"
#include <inttypes.h>

namespace {
using Real = FixedPoint<5, int>;

constexpr int kTaylorSeriesIterations = 15;

constexpr double factorial(double n) {
  if (n == 0) {
    return 1;
  } else {
    return n * factorial(n - 1);
  }
}

constexpr double pow(double x, double p) {
  if (p == 0) {
    return 1;
  } else {
    return x * pow(x, p - 1);
  }
}

constexpr double sin(double x) {
  const int iterations = kTaylorSeriesIterations;
  double result = 0;
  for (int n = 0; n < iterations; ++n) {
    result += pow(-1, n) / factorial(2 * n + 1) * pow(x, 2 * n + 1);
  }
  return result;
}

constexpr double cos(double x) {
  const int iterations = kTaylorSeriesIterations;
  double result = 0;
  for (int n = 0; n < iterations; ++n) {
    result += pow(-1, n) / factorial(2 * n) * pow(x, 2 * n);
  }
  return result;
}

constexpr double tau = 2 * 3.14159265;

#define TRIG_TABLE(function)                                                   \
  Real(function(tau * 0. / 256.)), Real(function(tau * 1. / 256.)),            \
      Real(function(tau * 2. / 256.)), Real(function(tau * 3. / 256.)),        \
      Real(function(tau * 4. / 256.)), Real(function(tau * 5. / 256.)),        \
      Real(function(tau * 6. / 256.)), Real(function(tau * 7. / 256.)),        \
      Real(function(tau * 8. / 256.)), Real(function(tau * 9. / 256.)),        \
      Real(function(tau * 10. / 256.)), Real(function(tau * 11. / 256.)),      \
      Real(function(tau * 12. / 256.)), Real(function(tau * 13. / 256.)),      \
      Real(function(tau * 14. / 256.)), Real(function(tau * 15. / 256.)),      \
      Real(function(tau * 16. / 256.)), Real(function(tau * 17. / 256.)),      \
      Real(function(tau * 18. / 256.)), Real(function(tau * 19. / 256.)),      \
      Real(function(tau * 20. / 256.)), Real(function(tau * 21. / 256.)),      \
      Real(function(tau * 22. / 256.)), Real(function(tau * 23. / 256.)),      \
      Real(function(tau * 24. / 256.)), Real(function(tau * 25. / 256.)),      \
      Real(function(tau * 26. / 256.)), Real(function(tau * 27. / 256.)),      \
      Real(function(tau * 28. / 256.)), Real(function(tau * 29. / 256.)),      \
      Real(function(tau * 30. / 256.)), Real(function(tau * 31. / 256.)),      \
      Real(function(tau * 32. / 256.)), Real(function(tau * 33. / 256.)),      \
      Real(function(tau * 34. / 256.)), Real(function(tau * 35. / 256.)),      \
      Real(function(tau * 36. / 256.)), Real(function(tau * 37. / 256.)),      \
      Real(function(tau * 38. / 256.)), Real(function(tau * 39. / 256.)),      \
      Real(function(tau * 40. / 256.)), Real(function(tau * 41. / 256.)),      \
      Real(function(tau * 42. / 256.)), Real(function(tau * 43. / 256.)),      \
      Real(function(tau * 44. / 256.)), Real(function(tau * 45. / 256.)),      \
      Real(function(tau * 46. / 256.)), Real(function(tau * 47. / 256.)),      \
      Real(function(tau * 48. / 256.)), Real(function(tau * 49. / 256.)),      \
      Real(function(tau * 50. / 256.)), Real(function(tau * 51. / 256.)),      \
      Real(function(tau * 52. / 256.)), Real(function(tau * 53. / 256.)),      \
      Real(function(tau * 54. / 256.)), Real(function(tau * 55. / 256.)),      \
      Real(function(tau * 56. / 256.)), Real(function(tau * 57. / 256.)),      \
      Real(function(tau * 58. / 256.)), Real(function(tau * 59. / 256.)),      \
      Real(function(tau * 60. / 256.)), Real(function(tau * 61. / 256.)),      \
      Real(function(tau * 62. / 256.)), Real(function(tau * 63. / 256.)),      \
      Real(function(tau * 64. / 256.)), Real(function(tau * 65. / 256.)),      \
      Real(function(tau * 66. / 256.)), Real(function(tau * 67. / 256.)),      \
      Real(function(tau * 68. / 256.)), Real(function(tau * 69. / 256.)),      \
      Real(function(tau * 70. / 256.)), Real(function(tau * 71. / 256.)),      \
      Real(function(tau * 72. / 256.)), Real(function(tau * 73. / 256.)),      \
      Real(function(tau * 74. / 256.)), Real(function(tau * 75. / 256.)),      \
      Real(function(tau * 76. / 256.)), Real(function(tau * 77. / 256.)),      \
      Real(function(tau * 78. / 256.)), Real(function(tau * 79. / 256.)),      \
      Real(function(tau * 80. / 256.)), Real(function(tau * 81. / 256.)),      \
      Real(function(tau * 82. / 256.)), Real(function(tau * 83. / 256.)),      \
      Real(function(tau * 84. / 256.)), Real(function(tau * 85. / 256.)),      \
      Real(function(tau * 86. / 256.)), Real(function(tau * 87. / 256.)),      \
      Real(function(tau * 88. / 256.)), Real(function(tau * 89. / 256.)),      \
      Real(function(tau * 90. / 256.)), Real(function(tau * 91. / 256.)),      \
      Real(function(tau * 92. / 256.)), Real(function(tau * 93. / 256.)),      \
      Real(function(tau * 94. / 256.)), Real(function(tau * 95. / 256.)),      \
      Real(function(tau * 96. / 256.)), Real(function(tau * 97. / 256.)),      \
      Real(function(tau * 98. / 256.)), Real(function(tau * 99. / 256.)),      \
      Real(function(tau * 100. / 256.)), Real(function(tau * 101. / 256.)),    \
      Real(function(tau * 102. / 256.)), Real(function(tau * 103. / 256.)),    \
      Real(function(tau * 104. / 256.)), Real(function(tau * 105. / 256.)),    \
      Real(function(tau * 106. / 256.)), Real(function(tau * 107. / 256.)),    \
      Real(function(tau * 108. / 256.)), Real(function(tau * 109. / 256.)),    \
      Real(function(tau * 110. / 256.)), Real(function(tau * 111. / 256.)),    \
      Real(function(tau * 112. / 256.)), Real(function(tau * 113. / 256.)),    \
      Real(function(tau * 114. / 256.)), Real(function(tau * 115. / 256.)),    \
      Real(function(tau * 116. / 256.)), Real(function(tau * 117. / 256.)),    \
      Real(function(tau * 118. / 256.)), Real(function(tau * 119. / 256.)),    \
      Real(function(tau * 120. / 256.)), Real(function(tau * 121. / 256.)),    \
      Real(function(tau * 122. / 256.)), Real(function(tau * 123. / 256.)),    \
      Real(function(tau * 124. / 256.)), Real(function(tau * 125. / 256.)),    \
      Real(function(tau * 126. / 256.)), Real(function(tau * 127. / 256.)),    \
      Real(function(tau * 128. / 256.)), Real(function(tau * 129. / 256.)),    \
      Real(function(tau * 130. / 256.)), Real(function(tau * 131. / 256.)),    \
      Real(function(tau * 132. / 256.)), Real(function(tau * 133. / 256.)),    \
      Real(function(tau * 134. / 256.)), Real(function(tau * 135. / 256.)),    \
      Real(function(tau * 136. / 256.)), Real(function(tau * 137. / 256.)),    \
      Real(function(tau * 138. / 256.)), Real(function(tau * 139. / 256.)),    \
      Real(function(tau * 140. / 256.)), Real(function(tau * 141. / 256.)),    \
      Real(function(tau * 142. / 256.)), Real(function(tau * 143. / 256.)),    \
      Real(function(tau * 144. / 256.)), Real(function(tau * 145. / 256.)),    \
      Real(function(tau * 146. / 256.)), Real(function(tau * 147. / 256.)),    \
      Real(function(tau * 148. / 256.)), Real(function(tau * 149. / 256.)),    \
      Real(function(tau * 150. / 256.)), Real(function(tau * 151. / 256.)),    \
      Real(function(tau * 152. / 256.)), Real(function(tau * 153. / 256.)),    \
      Real(function(tau * 154. / 256.)), Real(function(tau * 155. / 256.)),    \
      Real(function(tau * 156. / 256.)), Real(function(tau * 157. / 256.)),    \
      Real(function(tau * 158. / 256.)), Real(function(tau * 159. / 256.)),    \
      Real(function(tau * 160. / 256.)), Real(function(tau * 161. / 256.)),    \
      Real(function(tau * 162. / 256.)), Real(function(tau * 163. / 256.)),    \
      Real(function(tau * 164. / 256.)), Real(function(tau * 165. / 256.)),    \
      Real(function(tau * 166. / 256.)), Real(function(tau * 167. / 256.)),    \
      Real(function(tau * 168. / 256.)), Real(function(tau * 169. / 256.)),    \
      Real(function(tau * 170. / 256.)), Real(function(tau * 171. / 256.)),    \
      Real(function(tau * 172. / 256.)), Real(function(tau * 173. / 256.)),    \
      Real(function(tau * 174. / 256.)), Real(function(tau * 175. / 256.)),    \
      Real(function(tau * 176. / 256.)), Real(function(tau * 177. / 256.)),    \
      Real(function(tau * 178. / 256.)), Real(function(tau * 179. / 256.)),    \
      Real(function(tau * 180. / 256.)), Real(function(tau * 181. / 256.)),    \
      Real(function(tau * 182. / 256.)), Real(function(tau * 183. / 256.)),    \
      Real(function(tau * 184. / 256.)), Real(function(tau * 185. / 256.)),    \
      Real(function(tau * 186. / 256.)), Real(function(tau * 187. / 256.)),    \
      Real(function(tau * 188. / 256.)), Real(function(tau * 189. / 256.)),    \
      Real(function(tau * 190. / 256.)), Real(function(tau * 191. / 256.)),    \
      Real(function(tau * 192. / 256.)), Real(function(tau * 193. / 256.)),    \
      Real(function(tau * 194. / 256.)), Real(function(tau * 195. / 256.)),    \
      Real(function(tau * 196. / 256.)), Real(function(tau * 197. / 256.)),    \
      Real(function(tau * 198. / 256.)), Real(function(tau * 199. / 256.)),    \
      Real(function(tau * 200. / 256.)), Real(function(tau * 201. / 256.)),    \
      Real(function(tau * 202. / 256.)), Real(function(tau * 203. / 256.)),    \
      Real(function(tau * 204. / 256.)), Real(function(tau * 205. / 256.)),    \
      Real(function(tau * 206. / 256.)), Real(function(tau * 207. / 256.)),    \
      Real(function(tau * 208. / 256.)), Real(function(tau * 209. / 256.)),    \
      Real(function(tau * 210. / 256.)), Real(function(tau * 211. / 256.)),    \
      Real(function(tau * 212. / 256.)), Real(function(tau * 213. / 256.)),    \
      Real(function(tau * 214. / 256.)), Real(function(tau * 215. / 256.)),    \
      Real(function(tau * 216. / 256.)), Real(function(tau * 217. / 256.)),    \
      Real(function(tau * 218. / 256.)), Real(function(tau * 219. / 256.)),    \
      Real(function(tau * 220. / 256.)), Real(function(tau * 221. / 256.)),    \
      Real(function(tau * 222. / 256.)), Real(function(tau * 223. / 256.)),    \
      Real(function(tau * 224. / 256.)), Real(function(tau * 225. / 256.)),    \
      Real(function(tau * 226. / 256.)), Real(function(tau * 227. / 256.)),    \
      Real(function(tau * 228. / 256.)), Real(function(tau * 229. / 256.)),    \
      Real(function(tau * 230. / 256.)), Real(function(tau * 231. / 256.)),    \
      Real(function(tau * 232. / 256.)), Real(function(tau * 233. / 256.)),    \
      Real(function(tau * 234. / 256.)), Real(function(tau * 235. / 256.)),    \
      Real(function(tau * 236. / 256.)), Real(function(tau * 237. / 256.)),    \
      Real(function(tau * 238. / 256.)), Real(function(tau * 239. / 256.)),    \
      Real(function(tau * 240. / 256.)), Real(function(tau * 241. / 256.)),    \
      Real(function(tau * 242. / 256.)), Real(function(tau * 243. / 256.)),    \
      Real(function(tau * 244. / 256.)), Real(function(tau * 245. / 256.)),    \
      Real(function(tau * 246. / 256.)), Real(function(tau * 247. / 256.)),    \
      Real(function(tau * 248. / 256.)), Real(function(tau * 249. / 256.)),    \
      Real(function(tau * 250. / 256.)), Real(function(tau * 251. / 256.)),    \
      Real(function(tau * 252. / 256.)), Real(function(tau * 253. / 256.)),    \
      Real(function(tau * 254. / 256.)), Real(function(tau * 255. / 256.))

constexpr Real sin_table[] = {TRIG_TABLE(sin)};
constexpr Real cos_table[] = {TRIG_TABLE(cos)};

} // namespace

int Angle::sin(int r) { return (Real(r) * sin_table[val]).asInt(); 

}
int Angle::cos(int r) { return (Real(r) * cos_table[val]).asInt(); }